#!/usr/bin/env python3
import argparse
import sys
import xml.etree.ElementTree

#------------------------------------------------------------------------
class GLType:
    #--------------------------------------------------------------------
    def __init__(self, name, definition, api=None, requires=None):
        self.name = name
        self.definition = definition
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
        return self.name < other.name

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

    def __lt__(self, other):
        return self.name < other.name

#------------------------------------------------------------------------
class GLFeature:
    #--------------------------------------------------------------------
    def __init__(self, name, types, enums, commands):
        self.types = types
        self.name = name
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

        root = xml_tree.getroot()
        for child in root:
            handler = self.handlers.get(child.tag)
            if handler:
                handler(child)

    #--------------------------------------------------------------------
    def process_commands(self, commands_node):
        for command in commands_node:
            proto = command.find('proto')
            return_type = proto.text
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
        enums = []
        commands = []
        types = {}
        known_components = ('enum', 'command', 'type')
        for require in feature_node:
            for component in require:
                assert component.tag in known_components, 'Unexpected require child "{0}"'.format(component.tag)
                component_name = component.attrib['name']
                if component.tag == 'enum':
                    enum = self.enums[component_name]
                    enums.append(enum)
                elif component.tag == 'command':
                    command = self.commands[component_name]
                    commands.append(command)
                else:
                    types[component_name] = self.types[component_name]

        enums.sort()
        commands.sort()
        self.features[feature_name] = GLFeature(feature_name, types, enums, commands)
            
    #--------------------------------------------------------------------
    def process_groups(self, groups_node):
        for group in groups_node:
            assert group.tag == 'group', 'Unexpected groups child tag "{0}"'.format(group.tag)
            group_name = group.attrib['name']
            for enum in group:
                enum_name = enum.attrib['name']
                self.enums[enum_name] = GLEnum(enum_name, group_name)

    #--------------------------------------------------------------------
    def process_type_chunk(self, type_chunk):
        name = type_chunk.attrib['name']
        definition = type_chunk.text
        api = type_chunk.attrib.get('api')
        requires = type_chunk.attrib.get('requires')
        
        type_list = self.types.setdefault(name, [])
        type_list.append(GLType(name, definition, api=api, requires=requires))

    #--------------------------------------------------------------------
    def process_type(self, typeNode):
        definition = [typeNode.text or '']
        for child in typeNode:
            if child.tag == 'apientry':
                definition.append('APIENTRY')
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
        type_list.append(GLType(name, definition, api=api, requires=requires))

    #--------------------------------------------------------------------
    def process_types(self, types):
        for typeNode in types:
            if 'name' in typeNode.attrib:
                self.process_type_chunk(typeNode)
            else:
                self.process_type(typeNode)

#------------------------------------------------------------------------
def subcommand_list_features(parsed_args, spec):
    for i, feature_name in enumerate(sorted(spec.features)):
        feature = spec.features[feature_name]
        print('{0:2}'.format(i + 1), feature.name)

#------------------------------------------------------------------------
def subcommand_write_glsys(parsed_args, spec):
    feature = parsed_args.feature
    assert feature in spec.features, 'Unknown feature "{0}"'.format(feature)
    feature = spec.features[feature]
    print('types')
    for t in feature.types:
        print('\t' + t)
    print('enums/defines')
    for e in feature.enums:
        print('\t' + e.name)
    print('commands/functions')
    for c in feature.commands:
        print('\t' + c.name)
    
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
    write_glsys.add_argument('--header', metavar='FILE', required=True,
                             dest='header_filename', help='Output header filename')
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
