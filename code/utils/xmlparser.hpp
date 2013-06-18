#ifndef _GVN_XMLPARSER_HEAD_
#define _GVN_XMLPARSER_HEAD_

#include <graviton.hpp>
#include <core/logger.hpp>

#include <external/rapidxml/rapidxml.hpp>
#include <external/rapidxml/rapidxml_utils.hpp>
#include <external/rapidxml/rapidxml_print.hpp>
using namespace std;

namespace GraVitoN
{
    namespace Utils
    {
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        // typedef rapidxml::xml_node<> XML_Node;

        class XML_Attrib
        {
		private:
			rapidxml::xml_attribute<> *attr;
			
        public:
            XML_Attrib(rapidxml::xml_attribute<> * _attr = _null_) { attr = _attr; }
			~XML_Attrib() {}
			
			operator rapidxml::xml_attribute<> * ()
			{
				return attr;
			}
			
			bool operator ! ()
			{
				return attr == _null_;
			}
			
            XML_Attrib   next(const char *name = _null_)
            {
				if( !attr ) return XML_Attrib(_null_);
				return (XML_Attrib)(name?(attr->next_attribute(name)):(attr->next_attribute()));
			}

			char 		 *name()					  
			{
				if( !attr ) return _null_;
				return attr->name();
			}
            
			void         setName(const char *name)    
			{
				if( !attr ) return;
				attr->name( name );
			}
			
			char 		 *value()					  
			{
				if( !attr ) return _null_;
				return attr->value(); 
			}
            void         setValue(const char *value)  
			{
				if(!attr) return;
				attr->value( value );
			}
        };
        
        class XML_Node
        {
		private:
			rapidxml::xml_node<> *node;
			
        public:  
            XML_Node(rapidxml::xml_node<> *_node = _null_)
			{
				node = _node;
			}
			
			~XML_Node() {}
			
			bool operator ! ()
			{
				return node == _null_;
			}
			
			operator rapidxml::xml_node<> * ()
			{
				return node;
			}
			
            void removeAttribute(XML_Attrib &_attr)
			{
				if(!node) return;
				node->remove_attribute((rapidxml::xml_attribute<> *)_attr);
			}
            
			void removeAllAttributes()
            {
				if(!node) return;
				node->remove_all_attributes();
			}
            
			void removeChild(XML_Node &_node)
            {
				if(!node) return;
				node->remove_node( (rapidxml::xml_node<>*)_node );
			}
            
			void removeAllChilds()
            {
				if(!node) return;
				node->remove_all_nodes();
			}

            XML_Node     firstChild(const char *name = _null_)
            {
				if(!node) return XML_Node(_null_);
				return XML_Node(name?(node->first_node(name)):(node->first_node()));
			}
            
			XML_Node     next(const char *name = _null_)
            {
				if(!node) return XML_Node(_null_);
				return XML_Node(name?(node->next_sibling(name)):(node->next_sibling()));
			}
            XML_Attrib   firstAttribute(const char *name = _null_)
            {
				if(!node) return XML_Attrib(_null_);
				return XML_Attrib(name?(node->first_attribute(name)):(node->first_attribute()));
			}

			char		*name()
			{
				if(!node) return _null_;
				return node->name();
			}
			
            void         setName(const char *name)    
			{
				if(!node) return;
				node->name( name );
			} 
			
            void         setValue(const char *value)  
			{
				if(!node) return;
				node->value( value );
			}
			
			char 		*value()
			{
				if(!node) return _null_;
				return node->value();
			}
        };
            
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        class XML_Parser
        {
		private:
			std::vector<char> file_data;   // File data
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

            XML_Node firstNode(const char *name = _null_)
			{
				return XML_Node( xml_doc.first_node( name ) );
			}

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
			if(!decl) return false;
            decl->append_attribute(xml_doc.allocate_attribute("version", version));
            decl->append_attribute(xml_doc.allocate_attribute("encoding", encoding));
            xml_doc.append_node(decl);  
			return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::addNode(XML_Node &node, const char *name, const char *value)
        {
            node = XML_Node(xml_doc.clone_node(xml_doc.allocate_node(rapidxml::node_element, name, value)));

            if(!node) return false;
            xml_doc.append_node( (rapidxml::xml_node<>*)node );
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::addChild(XML_Node &node, XML_Node &child, const char*name, const char*value)
        {
            child = XML_Node(xml_doc.clone_node(xml_doc.allocate_node(rapidxml::node_element, name, value)));

            if(!child) return false;
            ((rapidxml::xml_node<>*)node)->append_node((rapidxml::xml_node<>*)child);
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::addAttribute(XML_Node &node, XML_Attrib &attr, const char*name, const char*value)
        {
            attr = XML_Attrib( xml_doc.allocate_attribute(name, value) );
            if(!attr) return false;
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
			try
			{
				xml_doc.parse<0>( text );
			}
			catch( ... )
			{
				return false;
			}
			return true;
		}
		
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Parser::load(const char *filename)
        {
			try
			{
				// Open stream
				basic_ifstream<char> stream(filename, ios::binary);
				if (!stream)
					throw runtime_error(string("cannot open file ") + filename);
				stream.unsetf(ios::skipws);
				
				// Determine stream size
				stream.seekg(0, ios::end);
				size_t size = stream.tellg();
				stream.seekg(0);   
				
				// Load data and add terminating 0
				file_data.resize(size + 1);
				stream.read(&file_data.front(), static_cast<streamsize>(size));
				file_data[size] = 0;
				
				return parse( &file_data.front());
			}
			catch(...)
			{
			}
			
            return false;
        }
    }
}

#endif
