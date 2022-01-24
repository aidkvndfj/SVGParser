#include "SVGHelpers.h"

#include "SVGParser.h"

void parseSVG(xmlNode* rootNode, SVG* currSVG, Group* currGroup) {
    xmlNode* currNode = NULL;

    for (currNode = rootNode; currNode != NULL; currNode = currNode->next) {
        if (currNode->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", currNode->name);
        }

        if (currNode->content != NULL) {
            printf("  content: %s\n", currNode->content);
        }

        // Iterate through every attribute of the current node
        xmlAttr* attr;
        for (attr = currNode->properties; attr != NULL; attr = attr->next) {
            xmlNode* value = attr->children;
            char* attrName = (char*)attr->name;
            char* cont = (char*)(value->content);
            printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
        }

        if (strcmp((char*)currNode->name, "title") == 0) {
            strcpy(currSVG->title, (char*)currNode->children->content);
        }
        if (strcmp((char*)currNode->name, "desc") == 0) {
            strcpy(currSVG->description, (char*)currNode->children->content);
        }

        parseSVG(currNode->children, currSVG, NULL);
    }
}
