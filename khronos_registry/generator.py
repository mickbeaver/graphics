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

#------------------------------------------------------------------------
class GLSpec:
    #--------------------------------------------------------------------
    def __init__(self, xmlTree):
        self.handlers = {'comment': self.process_comment,
                         'types': self.process_types}
        self.types = {}
        root = xmlTree.getroot()
        for child in root:
            handler = self.handlers.get(child.tag)
            if handler:
                handler(child)
        for k in self.types:
            print(k)
            for o in self.types[k]:
                print('  ', o)

    #--------------------------------------------------------------------
    def process_comment(self, comment):
        self.header_comment = comment.text

    #--------------------------------------------------------------------
    def process_type_chunk(self, typeChunk):
        name = typeChunk.attrib['name']
        definition = typeChunk.text
        api = typeChunk.attrib.get('api')
        requires = typeChunk.attrib.get('requires')
        
        typeList = self.types.setdefault(name, [])
        typeList.append(GLType(name, definition, api=api, requires=requires))

    #--------------------------------------------------------------------
    def process_type(self, typeNode):
        definition = [typeNode.text or '']
        for child in typeNode:
            if child.tag == 'apientry':
                definition.append('APIENTRY')
            elif child.tag == 'name':
                name = child.text
            else:
                assert 'Unhandled tag "{0}"'.format(child.tag)
            definition.append(child.text or '')
            definition.append(child.tail or '')
        definition = ''.join(definition)
            
        api = typeNode.attrib.get('api')
        requires = typeNode.attrib.get('requires')
        typeList = self.types.setdefault(name, [])
        typeList.append(GLType(name, definition, api=api, requires=requires))

    #--------------------------------------------------------------------
    def process_types(self, types):
        for typeNode in types:
            if 'name' in typeNode.attrib:
                self.process_type_chunk(typeNode)
            else:
                self.process_type(typeNode)

#------------------------------------------------------------------------
def parse_args(args):
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', metavar='FILE', required=True,
                        dest='input_filename', help='Input XML spec.')
    args = parser.parse_args(args)
    return args

#------------------------------------------------------------------------
def main(argv=None):
    if argv is None:
        argv = sys.argv[1:]
    args = parse_args(argv)
    tree = xml.etree.ElementTree.parse(args.input_filename)
    spec = GLSpec(tree)
        
#------------------------------------------------------------------------
if __name__ == '__main__':
    sys.exit(main())
