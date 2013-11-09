#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/HTMLparser.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlmemory.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
void parseHTMLtoGetURL( const char* fileName );
void parseAtag( xmlDocPtr doc, xmlNodePtr cur );
xmlXPathObjectPtr parsebyXPath( xmlDocPtr doc, xmlChar* xpath );
int main( int argc, char* argv[] )
{
	int i = 0;

	if( argc != 2 ){
		fprintf( stderr, "Command Line Argument Error\n" );
		return -1;
	}
	
	parseHTMLtoGetURL( argv[1] );

	return 0;
}

void parseHTMLtoGetURL( const char* fileName )
{
	int i = 0;
	xmlDocPtr doc;
	xmlChar* xpath = (xmlChar *)"//*/a";
	xmlXPathObjectPtr  result;
	xmlChar* keyword;
	
	doc = htmlParseFile( fileName, "UTF-8" );

	if( doc == NULL ){
		fprintf( stderr, "Document not parsed successfully.\n" );
		return;
	}

	result = parsebyXPath( doc, xpath );

	if( result == NULL ){
		fprintf( stderr, "Not Found XPath:%s\n", (const char *)xpath);
		return;
	}

	for( i = 0; i < result->nodesetval->nodeNr; i++ ){
		keyword = xmlNodeListGetString(doc, result->nodesetval->nodeTab[i]->xmlChildrenNode, 1);
		printf("%s:", keyword );
		xmlFree(keyword);
		keyword = xmlGetProp( result->nodesetval->nodeTab[i], (const xmlChar *)"href" );
		printf("%s\n", keyword );
		xmlFree(keyword);
	}

	xmlXPathFreeObject(result);

	return;
}

xmlXPathObjectPtr parsebyXPath( xmlDocPtr doc, xmlChar* xpath )
{
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;
	context = xmlXPathNewContext(doc);
	result = xmlXPathEvalExpression( xpath, context );
	xmlXPathFreeContext(context);
	if( xmlXPathNodeSetIsEmpty(result->nodesetval) ){
		xmlXPathFreeObject(result);
		return NULL;
	}
	return result;
}
