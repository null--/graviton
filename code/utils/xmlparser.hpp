#ifndef _GVN_XMLPARSER_HEAD_
#define _GVN_XMLPARSER_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>

#include <external/rapidxml/rapidxml.hpp>
#include <external/rapidxml/rapidxml_utils.hpp>
#include <external/rapidxml/rapidxml_print.hpp>
#include <map>
using namespace std;

namespace GraVitoN
{
    namespace Utils
    {
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        // typedef rapidxml::xml_node<> XML_Node;
        
        class XML_Attrib
        {
        public:
            rapidxml::xml_attribute<> *attr;

            XML_Attrib(rapidxml::xml_attribute<> *_attr = _null_) {attr = _attr;}

            operator rapidxml::xml_attribute<>* ()
                { return attr; }

            XML_Attrib   next(const char *name = _null_)
                {return (XML_Attrib)(name?(attr->next_attribute(name)):(attr->next_attribute()));}

            char*        name()                       {return attr->name();}
            void         setName(const char *name)    {attr->name( name );}

            char*        value()                      {return attr->value();}
            void         setValue(const char *value)  {attr->value( value );}

            bool isValid() {return attr != _null_;}
        };
            
        class XML_Node
        {
        public:
            rapidxml::xml_node<> *node;
            
            XML_Node(rapidxml::xml_node<> *_node = _null_) {node = _node;}
            
            operator rapidxml::xml_node<> * ()
                { return node; }

            void removeAttribute(XML_Attrib &_attr)
                {node->remove_attribute((rapidxml::xml_attribute<> *)_attr); }
            void removeAllAttributes()
                {node->remove_all_attributes();}
            void removeChild(XML_Node &_node)
                {node->remove_node( _node.node );}
            void removeAllChilds()
                {node->remove_all_nodes();}

            XML_Node     firstChild(const char *name = _null_)
                {return (XML_Node)(name?(node->first_node(name)):(node->first_node()));}
            XML_Node     next(const char *name = _null_)
                {return (XML_Node)(name?(node->next_sibling(name)):(node->next_sibling()));}
            XML_Attrib   firstAttribute(const char *name = _null_)
                {return (XML_Attrib)(name?(node->first_attribute(name)):(node->first_attribute()));}

            char*        name()                       {return node->name(); }
            void         setName(const char *name)    {node->name( name ); } 
            
            char*        value()                      {return node->value();}
            void         setValue(const char *value)  {node->value( value );}

            bool isValid() {return node != _null_;}
        };
            
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        class XML_Parser
        {
        private:
            rapidxml::xml_document<> xml_doc;

        public:
            XML_Parser();
            ~XML_Parser();

            bool addDecleration(const char *version = "0.1", const char *encoding = "UTF-8");
            bool addNode(XML_Node &node, const char *name, const char *value = "");
            bool addAttribute(XML_Node &node, XML_Attrib &attr, const char*name, const char*value);
            bool addChild(XML_Node &node, XML_Node &child, const char*name, const char*value = "");

            bool save(const char *path);
            bool load(const char *path);
            bool parse(char *path);

            XML_Node firstNode(const char *name = _null_)   {return (XML_Node)xml_doc.first_node( name ); }

            void clear() {xml_doc.clear();}
            // XML_Node *cloneNode(const XML_Node *&node) {return xml_doc.clone_node(node); }

            void removeNode(XML_Node &node) {xml_doc.remove_node( (rapidxml::xml_node<>*)node );}
            void removeAllNodes() {xml_doc.remove_all_nodes();}
        };

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        XML_Parser::XML_Parser()
        {
            // xml_doc.set_allocator(malloc, free);
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        XML_Parser::~XML_Parser()
        {
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::addDecleration(const char *version, const char *encoding)
        {
            rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
            decl->append_attribute(xml_doc.allocate_attribute("version", version));
            decl->append_attribute(xml_doc.allocate_attribute("encoding", encoding));
            xml_doc.append_node(decl);  
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::addNode(XML_Node &node, const char *name, const char *value)
        {
            if( !value || value[0] == '\0' )
                node.node = xml_doc.allocate_node(rapidxml::node_element, name);
            else
                node.node = xml_doc.allocate_node(rapidxml::node_element, name, value);

            if(!node.node) return false;
            xml_doc.append_node( (rapidxml::xml_node<>*)node);
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::addChild(XML_Node &node, XML_Node &child, const char*name, const char*value)
        {
            if( !value || value[0] == '\0' )
                child.node = xml_doc.allocate_node(rapidxml::node_element, name);
            else
                child.node = xml_doc.allocate_node(rapidxml::node_element, name, value);

            if(!child.node) return false;
            ((rapidxml::xml_node<>*)node)->append_node((rapidxml::xml_node<>*)child);
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::addAttribute(XML_Node &node, XML_Attrib &attr, const char*name, const char*value)
        {
            attr = (XML_Attrib)xml_doc.allocate_attribute(name, value);
            if(!attr.attr) return false;
            ((rapidxml::xml_node<>*)node)->append_attribute((rapidxml::xml_attribute<>*)attr);
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::save(const char *path)
        {
            ofstream ofile;
            ofile.open(path);
            ofile << xml_doc;
            ofile.close();
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::parse(char *text)
        {
            xml_doc.parse<0>(text);
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::load(const char *path)
        {
            rapidxml::file<char> xml_file(path);
            bool res = parse( xml_file.data() );
            // xml_file.close();
            return res;
        }
    }
}

#endif
