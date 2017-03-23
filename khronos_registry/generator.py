#!/usr/bin/env python3
import argparse
import copy
import contextlib
import os
import sys
import xml.etree.ElementTree

#------------------------------------------------------------------------
class GLType:
    #--------------------------------------------------------------------
    def __init__(self, name, definition, sort_id, api='gl', requires=None):
        self.name = name
        self.definition = definition
        self.sort_id = sort_id
        self.api = api
        self.requires = requires

    #--------------------------------------------------------------------
    def __str__(self):
        definition = ' '.join(self.definition.splitlines())
        if len(definition) > 32:
            definition = definition[:32] + '...'
        format_dict = {'name': self.name,
                       'definition': self.definition[:32],
                       'api': self.api,
                       'requires': self.requires}
        return 'GLType({name}, "{definition}", api={api}, requires={requires})'.format(**format_dict)

    #--------------------------------------------------------------------
    def __lt__(self, other):
        return self.sort_id < other.sort_id

#------------------------------------------------------------------------
class GLEnum:
    #--------------------------------------------------------------------
    def __init__(self, name, group, value=None):
        self.name = name
        self.group = group
        self.value = value
    
    #--------------------------------------------------------------------
    def __lt__(self, other):
        return self.name < other.name

#------------------------------------------------------------------------
class GLCommandParameter:
    #--------------------------------------------------------------------
    def __init__(self, param_name, param_type):
        self.name = param_name
        self.type = param_type

    #--------------------------------------------------------------------
    def __str__(self):
        return 'GLCommandParameter({0}, {1})'.format(self.name, self.type)

    #--------------------------------------------------------------------
    def __repr__(self):
        return 'GLCommandParameter({0}, {1})'.format(self.name, self.type)

#------------------------------------------------------------------------
class GLCommand:
    #--------------------------------------------------------------------
    def __init__(self, name, return_type, parameters):
        self.name = name
        self.return_type = return_type
        self.parameters = parameters

    #--------------------------------------------------------------------
    def __lt__(self, other):
        return self.name < other.name

    #--------------------------------------------------------------------
    def get_func_decl(self):
        params = [x.type + ' ' + x.name for x in self.parameters]
        param_str = ', '.join(params)
        return '{0} {1}({2});'.format(self.return_type,
                                      self.name,
                                      param_str)

    #--------------------------------------------------------------------
    def get_func_ptr_name(self):
        return 'PFN' + self.name.upper() + 'PROC'

    #--------------------------------------------------------------------
    def get_func_ptr_typedef(self):
        params = [x.type + ' ' + x.name for x in self.parameters]
        param_str = ', '.join(params)
        return 'typedef {0} (* {1})({2});'.format(self.return_type,
                                                 self.get_func_ptr_name(),
                                                 param_str)

#------------------------------------------------------------------------
class GLFeature:
    #--------------------------------------------------------------------
    def __init__(self, name, api, types, enums, commands):
        self.name = name
        self.api = api
        self.types = types
        self.enums = enums
        self.commands = commands

#------------------------------------------------------------------------
class GLSpec:
    #--------------------------------------------------------------------
    def __init__(self, xml_tree):
        self.handlers = {'commands': self.process_commands,
                         'comment': self.process_comment,
                         'enums': self.process_enums,
                         'feature': self.process_feature,
                         'groups': self.process_groups,
                         'types': self.process_types}
        self.types = {}
        self.enums = {}
        self.commands = {}
        self.features = {}
        self.__temp_api = {} # temp store for running api

        root = xml_tree.getroot()
        for child in root:
            handler = self.handlers.get(child.tag)
            if handler:
                handler(child)

        del self.__temp_api

    #--------------------------------------------------------------------
    def process_commands(self, commands_node):
        for command in commands_node:
            proto = command.find('proto')
            ptype_node = proto.find('ptype')
            if ptype_node is None:
                return_type = proto.text or 'void'
            else:
                return_type = ''.join((proto.text or '',
                                       ptype_node.text or '',
                                       ptype_node.tail or ''))
            return_type = return_type.strip()
            name_node = proto.find('name')
            command_name = name_node.text

            param_objs = []
            params = command.findall('param')
            for param in params:
                ptype_node = param.find('ptype')
                if ptype_node is None:
                    param_type = param.text
                else:
                    param_type = ''.join((param.text or '',
                                          ptype_node.text or '',
                                          ptype_node.tail or '')).rstrip()
                param_name = param.find('name').text
                param_objs.append(GLCommandParameter(param_name, param_type))

            self.commands[command_name] = GLCommand(command_name,
                                                    return_type,
                                                    param_objs)

    #--------------------------------------------------------------------
    def process_comment(self, comment):
        self.header_comment = comment.text

    #--------------------------------------------------------------------
    def process_enums(self, enums_node):
        group_name = enums_node.get('group')
        for enum in enums_node:
            if enum.tag == 'unused':
                continue
            assert enum.tag == 'enum', 'Unexpected enums child tag "{0}"'.format(enum.tag)
            enum_name = enum.attrib['name']
            enum_value = enum.attrib['value']
            if enum_name in self.enums:
                self.enums[enum_name].value = enum_value
            else:
                self.enums[enum_name] = GLEnum(enum_name, group_name, enum_value)

    #--------------------------------------------------------------------
    def process_feature(self, feature_node):
        feature_name = feature_node.attrib['name']
        feature_api = feature_node.attrib['api']
        running_api = self.__temp_api.setdefault(feature_api, {'enum': set(),
                                                               'command': set(),
                                                               'type': set()})
        known_components = ('enum', 'command', 'type')
        for operator in feature_node:
            operation = operator.tag
            assert operation in ('require', 'remove')
            for component in operator:
                assert component.tag in known_components, 'Unexpected require/remove child "{0}"'.format(component.tag)
                component_name = component.attrib['name']
                if operation == 'require':
                    running_api[component.tag].add(component_name)
                else:
                    running_api[component.tag].remove(component_name)

        self.features[feature_name] = GLFeature(feature_name,
                                                feature_api,
                                                sorted(running_api['type']),
                                                sorted(running_api['enum']),
                                                sorted(running_api['command']))
            
    #--------------------------------------------------------------------
    def process_groups(self, groups_node):
        for group in groups_node:
            assert group.tag == 'group', 'Unexpected groups child tag "{0}"'.format(group.tag)
            group_name = group.attrib['name']
            for enum in group:
                enum_name = enum.attrib['name']
                self.enums[enum_name] = GLEnum(enum_name, group_name)

    #--------------------------------------------------------------------
    def process_type_chunk(self, type_chunk, sort_id):
        name = type_chunk.attrib['name']
        definition = type_chunk.text
        api = type_chunk.attrib.get('api')
        requires = type_chunk.attrib.get('requires')
        
        type_list = self.types.setdefault(name, [])
        type_list.append(GLType(name, definition, sort_id, api=api, requires=requires))

    #--------------------------------------------------------------------
    def process_type(self, typeNode, sort_id):
        definition = [typeNode.text or '']
        for child in typeNode:
            if child.tag == 'apientry':
                pass # ignored for now
                #definition.append('APIENTRY')
            elif child.tag == 'name':
                name = child.text
            else:
                assert False, 'Unhandled type child tag "{0}": {1}'.format(child.tag, child)
            definition.append(child.text or '')
            definition.append(child.tail or '')
        definition = ''.join(definition)
            
        api = typeNode.attrib.get('api')
        requires = typeNode.attrib.get('requires')
        type_list = self.types.setdefault(name, [])
        type_list.append(GLType(name, definition, sort_id, api=api, requires=requires))

    #--------------------------------------------------------------------
    def process_types(self, types):
        for sort_id, typeNode in enumerate(types):
            if 'name' in typeNode.attrib:
                self.process_type_chunk(typeNode, sort_id)
            else:
                self.process_type(typeNode, sort_id)

#------------------------------------------------------------------------
def subcommand_list_features(parsed_args, spec):
    for i, feature_name in enumerate(sorted(spec.features)):
        print('{0:2}'.format(i + 1), feature_name)

#------------------------------------------------------------------------
def subcommand_write_glsys(parsed_args, spec):
    #--------------------------------------------------------------------
    def get_closest_type(api, type_name):
        type_name = type_name.replace('const', '').replace('*', '').strip()
        type_list = spec.types.get(type_name)
        assert type_list, 'Did not get type list for "{0}"'.format(type_name)

        # If there is a type_obj that matches our API, we want that.
        # Otherwise, the default type definition should be the first
        # item in the list.
        for type_obj in type_list:
            if type_obj.api == api:
                return type_obj
        return type_list[0]

    #--------------------------------------------------------------------
    def calculate_types(api, feature_types, command_names):
        type_names_needed = set(feature_types)

        # For each function we need, iterate over the return type and
        # the parameters to grab necessary "GL"-prefixed types
        for command_name in command_names:
            command = spec.commands.get(command_name)
            types_to_check = [command.return_type] + [x.type for x in command.parameters]
            for type_to_check in types_to_check:
                for token in type_to_check.split():
                    if token.startswith('GL'):
                        type_names_needed.add(token)

        type_prereqs = set()
        types_needed = set()
        # Get types that we explicitly need via require and our commands
        for type_name in type_names_needed:
            closest_type = get_closest_type(api, type_name)
            types_needed.add(closest_type)
            if closest_type.requires:
                # Just one level of prereq, not an actual tree
                type_prereqs.add(closest_type.requires)

        # Get types we are dependent on
        for type_name in type_prereqs:
            closest_type = get_closest_type(api, type_name)
            types_needed.add(closest_type)

        return sorted(types_needed)

    #--------------------------------------------------------------------
    def write_glsys_header(filename, feature, lib_feature):
        separator = '//' + ('-' * 70)
        def print_sep(*args, **kwargs):
            print(separator)
            print(*args, **kwargs)
            print(separator)

        types = calculate_types(feature.api, feature.types, feature.commands)
        feature_func_names = set(feature.commands)
        lib_func_names = ((lib_feature and set(lib_feature.commands) & feature_func_names)
                          or set())
        feature_func_names -= lib_func_names
        lib_func_names = sorted(lib_func_names)
        feature_func_names = sorted(feature_func_names)

        output_obj = open(filename, 'w')
        with contextlib.redirect_stdout(output_obj):
            print('/**')
            print(' * Below is the copyright that came with the spec XML:')
            for line in spec.header_comment.splitlines():
                print(' *', line)
            print(' */')
            print_sep('// Custom header for minimal feature', feature.name)
            include_guard = 'GL_SYS_GENERATED_H_{0}_INCLUDED'.format(feature.name)
            print('#ifndef', include_guard)
            print('#define', include_guard, end='\n\n')
            print('#ifdef __cplusplus')
            print('extern "C" {')
            print('#endif // __cplusplus', end='\n\n')

            # Our selected feature
            print('// Known features')
            column_width = len(max(spec.features, key=len))
            feature_names = sorted(spec.features)
            for i, feature_name in enumerate(feature_names):
                print('#define {0:{width}} {1}'.format(feature_name, i, width=column_width))
            print('// Selected feature')
            print('#define GLSYS_FEATURE_VERSION', feature.name)
            major, minor = feature.name.split('_')[-2:]
            print('#define GLSYS_FEATURE_MAJOR_VERSION', major)
            print('#define GLSYS_FEATURE_MINOR_VERSION', minor)
            print('#define GLSYS_FEATURE_VERSION_STRING "{0}"'.format(feature.name), end='\n\n')

            # OpenGL types
            for type_obj in types:
                  print(type_obj.definition)
            print()

            # OpenGL function pointer typedefs
            feature_commands = [spec.commands.get(x) for x in feature_func_names]
            for command in feature_commands:
                print(command.get_func_ptr_typedef())
            print()

            # Enums/defines
            column_width = len(max(feature.enums, key=len))
            for enum_name in feature.enums:
                enum = spec.enums.get(enum_name)
                definition = '#define {0:{width}} {1}'.format(enum.name, enum.value, width=column_width)
                print(definition)
            print()
            
            print_sep('// Function intialization/loading (single context only!)')
            print('typedef void (*glsysFuncPtr)();')
            print('typedef glsysFuncPtr (*glsysFunctionLoader)(const char*);')
            print('int glsysLoadFunctions(glsysFunctionLoader functionLoader);', end='\n\n')

            # Functions contained in our system lib
            if lib_func_names:
                print_sep('// Functions contained in our system lib for feature',
                          lib_feature.name,
                          file=output_obj)
            for func_name in lib_func_names:
                command = spec.commands.get(func_name)
                print(command.get_func_decl())
            print()

            # Function pointers to load from the driver
            column_width = max([len(x.get_func_ptr_name()) for x in feature_commands], default=0)
            if feature_commands:
                print_sep('// Function pointers to be retrieved for feature',
                          feature.name)
            for command in feature_commands:
                line = 'extern {0:{width}} {1};'.format(command.get_func_ptr_name(),
                                                        command.name,
                                                        width=column_width)
                print(line)
            
            print('#ifdef __cplusplus')
            print('}')
            print('#endif // __cplusplus')
            print('#endif //', include_guard)
        output_obj.close()
        
    #--------------------------------------------------------------------
    def write_glsys_c_module(filename, header_filename, feature, lib_feature):
        feature_func_names = set(feature.commands)
        lib_func_names = ((lib_feature and set(lib_feature.commands) & feature_func_names)
                          or set())
        feature_func_names -= lib_func_names
        lib_func_names = sorted(lib_func_names)
        feature_func_names = sorted(feature_func_names)

        output_obj = open(filename, 'w')
        with contextlib.redirect_stdout(output_obj):
            print('#include <assert.h>')
            print('#include "{0}"\n'.format(os.path.basename(header_filename)))
            feature_commands = [spec.commands.get(x) for x in feature_func_names]
            col_width = max([len(x.get_func_ptr_name()) for x in feature_commands], default=0)

            for command in feature_commands:
                line = '{0:{width}} {1};'.format(command.get_func_ptr_name(),
                                                 command.name,
                                                 width=col_width)
                print(line)
            print()

            print('int glsysLoadFunctions(glsysFunctionLoader functionLoader)')
            print('{')
            col_width = max([len(x.name) for x in feature_commands], default=0)
            for command in feature_commands:
                line = '    {name:{width}} = ({type})functionLoader("{name}");'
                line = line.format(name=command.name,
                                   type=command.get_func_ptr_name(),
                                   width=col_width)
                print(line)
            print()
            for command in feature_commands:
                line = '    assert({name:{width}} != NULL);'
                line = line.format(name=command.name,
                                   width=col_width)
                print(line)
            print('    return 0;')
            print('}')

    #--------------------------------------------------------------------
    # Feature level we are targeting
    feature = parsed_args.feature
    assert feature in spec.features, 'Unknown feature "{0}"'.format(feature)
    feature = spec.features[feature]
    
    # Feature level contained in our .dll/.so
    lib_feature = parsed_args.lib_feature
    lib_feature = spec.features.get(lib_feature)

    write_glsys_header(parsed_args.header_filename, feature, lib_feature)
    write_glsys_c_module(parsed_args.c_module_filename,
                         parsed_args.header_filename,
                         feature,
                         lib_feature)
    
#------------------------------------------------------------------------
def parse_args(args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', metavar='FILE', required=True,
                        dest='input_filename', help='Input XML spec.')
    subparsers = parser.add_subparsers()

    list_features = subparsers.add_parser('list-features')
    list_features.set_defaults(subcommand_func=subcommand_list_features)

    write_glsys = subparsers.add_parser('write-glsys')
    write_glsys.add_argument('--feature', metavar='NAME', required=True,
                             dest='feature', help='Feature name')
    write_glsys.add_argument('--lib-feature', metavar='NAME',
                              dest='lib_feature',
                              help='Feature level contained in OpenGL lib')
    write_glsys.add_argument('--header', metavar='FILE', required=True,
                             dest='header_filename', help='Output header filename')
    write_glsys.add_argument('--c-module', metavar='FILE', required=True,
                             dest='c_module_filename', help='Output C module filename')
    write_glsys.set_defaults(subcommand_func=subcommand_write_glsys)

    args = parser.parse_args(args)
    return args

#------------------------------------------------------------------------
def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]
    parsed_args = parse_args(argv)
    tree = xml.etree.ElementTree.parse(parsed_args.input_filename)
    spec = GLSpec(tree)
    if 'subcommand_func' in parsed_args:
        parsed_args.subcommand_func(parsed_args, spec)

    return 0
        
#------------------------------------------------------------------------
if __name__ == '__main__':
    sys.exit(main())
