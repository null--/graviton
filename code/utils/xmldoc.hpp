/**
 * @file
 *
 * @author  Sina Hatef Matbue ( _null_ ) <sinahatef.cpp@gmail.com>
 *
 * @section License
 *
 * This file is part of GraVitoN.
 *
 * Graviton is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graviton is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Graviton.  If not, see http://www.gnu.org/licenses/.
 *
 * @brief XML
 *
 */

#ifndef GRAVITON_XMLPARSER_H
#define GRAVITON_XMLPARSER_H

#include <graviton.hpp>
#include <core/logger.hpp>

#include <external/rapidxml/rapidxml.hpp>
#include <external/rapidxml/rapidxml_utils.hpp>
#include <external/rapidxml/rapidxml_print.hpp>

namespace GraVitoN
{
    namespace Utils
    {
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /// xml attribute
        class XML_Attrib
        {
		private:
			rapidxml::xml_attribute<> *attr;
            char *m_name;
            char *m_value;
			
        public:
            XML_Attrib(rapidxml::xml_attribute<> * _attr = _null_)
                {
                    m_name = new char[ Config::MAX_XML_VALUE_SIZE ];
                    m_value = new char[ Config::MAX_XML_VALUE_SIZE ];
                    
                    attr = _attr;
                    if( attr )
                    {
                        /// change pointers
                        if( attr->name() )
                        {
                            strncpy(m_name, attr->name(), Config::MAX_XML_VALUE_SIZE - 1);
                            attr->name( m_name );
                        }
                        else
                        {
                            m_name = _null_;
                        }

                        if( attr->value() )
                        {
                            strncpy(m_value, attr->value(), Config::MAX_XML_VALUE_SIZE - 1);
                            attr->value( m_value );
                        }
                        else
                        {
                            m_value = _null_;
                        }
                    }
                }
                    
			~XML_Attrib()
                {
                    /// No Garbage Collecting For This Junks
                    attr = _null_;
                    m_name = _null_;
                    m_value = _null_;
                }

            /// cast XML_Attrib* to xml_attribute<>*
			operator rapidxml::xml_attribute<> * () const
                {
                    return attr;
                }

            /// check if attribute is valid
			bool operator ! () const
                {
                    return attr == _null_;
                }

            /** get next attribute
             * @params [IN]
             * name of attribute, use a empty string as argument when you want to retrive next-nearest attribute.
             */
            XML_Attrib   next(const std::string &name = "") const
                {
                    if( !attr ) return XML_Attrib(_null_);
                    return (XML_Attrib)(!name.empty()?(attr->next_attribute(name.c_str())):(attr->next_attribute()));
                }

            /// get attribute name
            std::string name() const					  
                {
                    if( !attr ) return "";
                    // m_name = attr->name();
                    return m_name;
                }

            /// set attribute name
			void setName(const std::string &name_)    
                {
                    if( !attr ) return;
                    strncpy(m_name, name_.c_str(), Config::MAX_XML_VALUE_SIZE - 1);
                    attr->name( m_name );
                }

            /// get attribute value
			string value() const		  
                {
                    if( !attr ) return "";
                    // m_value = attr->value();
                    return m_value; 
                }

            /// set attribute value
            void setValue(const std::string &value_)  
                {
                    if(!attr) return;
                    strncpy(m_value, value_.c_str(), Config::MAX_XML_VALUE_SIZE - 1);
                    attr->value( m_value );
                }
        };

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /// xml node
        class XML_Node
        {
		private:
			rapidxml::xml_node<> *node;
            char *m_name;
            char *m_value;
            
        public:  
            XML_Node(rapidxml::xml_node<> *_node = _null_)
                {
                    m_name = new char[ Config::MAX_XML_VALUE_SIZE ];
                    m_value = new char[ Config::MAX_XML_VALUE_SIZE ];
                    
                    node = _node;
                    if( node )
                    {
                        /// change pointers
                        if( node->name() )
                        {
                            strncpy(m_name, node->name(), Config::MAX_XML_VALUE_SIZE - 1);
                            // cout << "Adding node name = " << m_name << endl;
                            node->name( m_name );
                        }
                        else
                        {
                            m_name = _null_;
                        }

                        if( node->value() )
                        {
                            strncpy(m_value, node->value(), Config::MAX_XML_VALUE_SIZE - 1);
                            node->value( m_value );
                        }
                        else
                        {
                            m_value = _null_;
                        }
                    }
                }
			
			~XML_Node()
                {
                    /// No Garbage Collecting For This Junks
                    node = _null_;
                    m_name = _null_;
                    m_value = _null_;
                }

            /// check if node is valid
			bool operator ! () const
                {
                    return node == _null_;
                }

            /// cast XML_Node* to rapidxml::xml_node<>*
			operator rapidxml::xml_node<> * () const
                {
                    return node;
                }

            /// remove an attribute from node
            void removeAttribute(XML_Attrib &_attr)
                {
                    if(!node) return;
                    node->remove_attribute((rapidxml::xml_attribute<> *)_attr);
                }

            /// remove all attributes of node
			void removeAllAttributes()
                {
                    if(!node) return;
                    node->remove_all_attributes();
                }

            /// remove a child from ndoe
			void removeChild(XML_Node &_node)
                {
                    if(!node) return;
                    node->remove_node( (rapidxml::xml_node<>*)_node );
                }

            /// remove all childs of node
			void removeAllChilds()
                {
                    if(!node) return;
                    node->remove_all_nodes();
                }

            /// get first child of node (with or whitout a given name)
            XML_Node     firstChild(const std::string name = "") const
                {
                    if(!node) return XML_Node(_null_);
                    return XML_Node(!name.empty()?(node->first_node(name.c_str())):(node->first_node()));
                }

            /// get next sibling node
			XML_Node     next(const std::string &name = "") const
                {
                    if(!node) return XML_Node(_null_);
                    return XML_Node(!name.empty()?(node->next_sibling(name.c_str())):(node->next_sibling()));
                }

            /// get first attribute of node
            XML_Attrib   firstAttribute(const std::string &name = "") const
                {
                    if(!node) return XML_Attrib(_null_);
                    return XML_Attrib(!name.empty()?(node->first_attribute(name.c_str())):(node->first_attribute()));
                }

            /// get name of the node
            std::string name() const
                {
                    if(!node) return _null_;
                    // m_name = node->name();
                    return m_name;
                }

            /// set name of the node
            void setName(const std::string &name_)    
                {
                    if(!node) return;
                    strncpy(m_name, name_.c_str(), Config::MAX_XML_VALUE_SIZE - 1);
                    node->name( m_name );
                } 

            /// set value of the node
            void setValue(const std::string &value_)  
                {
                    if(!node) return;
                    strncpy(m_value, value_.c_str(), Config::MAX_XML_VALUE_SIZE - 1);
                    node->value( m_value );
                }

            /// get value of the node
            std::string value() const
                {
                    if(!node) return _null_;
                    // m_value = node->value();
                    return m_value;
                }

            /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
            bool addChild(XML_Node &child, const std::string &name, const std::string &value = "")
                {
                    if( !node ) return false;
                    rapidxml::xml_document<> *xml_doc = node->document();
                    if(!xml_doc) return false;
                    
                    child = XML_Node(
                        xml_doc->allocate_node(
                            rapidxml::node_element,
                            name.c_str(),
                            value.c_str()
                            )
                        );

                    if(!child) return false;
                    node->append_node((rapidxml::xml_node<>*)child);
                    return true;
                }

            /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
            bool addAttribute(XML_Attrib &attr, const std::string &name, const std::string &value)
                {
                    if( !node ) return false;
                    rapidxml::xml_document<> *xml_doc = node->document();
                    if(!xml_doc) return false;
                    
                    attr = XML_Attrib(
                        xml_doc->allocate_attribute(
                            name.c_str(),
                            value.c_str()
                            )
                        );
                    if(!attr) return false;
                    node->append_attribute((rapidxml::xml_attribute<>*)attr);
                    return true;
                }
        };
            
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        /// xml document
        class XML_Document
        {
		private:
			std::string file_data;   // File data
			rapidxml::xml_document<> xml_doc;
            string m_version;
            string m_encoding;
            
        public:
            XML_Document();
            ~XML_Document();

            /// add standard xml declaraion to a xml document
            bool addDecleration(const std::string &version = "0.1", const std::string &encoding = "UTF-8");
            /// add new node to document @see see also XML_Document::addChild
            bool addNode(XML_Node &node, const std::string &name, const std::string &value = "");

            /// save xml doc to an xml file
            bool save(const std::string &path);
            /// load doc from an xml file
            bool load(const std::string &path);
            /// parse a 
            bool parse(std::string &path);

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
        XML_Document::XML_Document()
        {
            // xml_doc.set_allocator(malloc, free);
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        XML_Document::~XML_Document()
        {
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Document::addDecleration(const std::string &version, const std::string &encoding)
        {
            m_version = version;
            m_encoding = encoding;
            
            rapidxml::xml_node<>* decl = xml_doc.allocate_node(rapidxml::node_declaration);
			if(!decl) return false;
            decl->append_attribute(xml_doc.allocate_attribute("version", m_version.c_str()));
            decl->append_attribute(xml_doc.allocate_attribute("encoding", m_encoding.c_str()));
            xml_doc.append_node(decl);  
			return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Document::addNode(XML_Node &node, const std::string &name, const std::string &value)
        {
            node = XML_Node(
                xml_doc.allocate_node(
                    rapidxml::node_element,
                    name.c_str(),
                    value.c_str()
                    )
                );

            if(!node) return false;
            xml_doc.append_node( (rapidxml::xml_node<>*)node );
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Document::save(const std::string &path)
        {
            ofstream ofile;
            ofile.open(path.c_str());
            ofile << xml_doc;
            ofile.close();
            return true;
        }

        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
		bool XML_Document::parse(std::string &text)
        {
			try
			{
				xml_doc.parse<0>( &text[0] );
			}
			catch( ... )
			{
				return false;
			}
			return true;
		}
		
        /* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
        bool XML_Document::load(const std::string &filename)
        {
			try
			{
				// Open stream
				basic_ifstream<char> stream(filename.c_str(), ios::binary);
				if (!stream)
					throw runtime_error(string("cannot open file ") + filename);
				stream.unsetf(ios::skipws);
				
				// Determine stream size
				stream.seekg(0, ios::end);
				size_t size = stream.tellg();
				stream.seekg(0);   
				
				// Load data and add terminating 0
				file_data.resize(size + 1);
				stream.read(&file_data[0], static_cast<streamsize>(size));
				file_data[size] = '\0';
				
				return parse( file_data );
			}
			catch(...)
			{
			}
			
            return false;
        }
    }
}

#endif
