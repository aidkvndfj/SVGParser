#include "SVGHelpers.h"

#include "SVGParser.h"

void parseSVG(xmlNode* rootNode, SVG* currSVG, Group* currGroup) {
    xmlNode* currNode = NULL;

    // loop through all the nodes
    for (currNode = rootNode; currNode != NULL; currNode = currNode->next) {
        // print out the node name and content for reference
        if (currNode->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", currNode->name);
        }
        if (currNode->content != NULL) {
            printf("  content: %s\n", currNode->content);
        }

        // Iterate through every attribute of the current node for reference
        xmlAttr* attr;
        for (attr = currNode->properties; attr != NULL; attr = attr->next) {
            xmlNode* value = attr->children;
            char* attrName = (char*)attr->name;
            char* cont = (char*)(value->content);
            printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
        }

        // if the node is the title, set the SVG title to the childrens content, aka the title content.
        if (strcmp((char*)currNode->name, "title") == 0) {
            strcpy(currSVG->title, (char*)currNode->children->content);
        }
        // if the node is the description, set the SVG dec. to the childrens content, aka the desc. content.
        else if (strcmp((char*)currNode->name, "desc") == 0) {
            strcpy(currSVG->description, (char*)currNode->children->content);
        }
        // if the node is a rectangle
        else if (strcmp((char*)currNode->name, "rect") == 0) {
            xmlAttr* attr;                                                                                     // create attr variable to keep track of the attributes
            Rectangle* newRect = malloc(sizeof(Rectangle));                                                    // create a new rect struct.
            newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initalize the attribute list
            // loop through all the attributes
            for (attr = currNode->properties; attr != NULL; attr = attr->next) {
                xmlNode* value = attr->children;  // create another node to get the content easier

                if (strcmp((char*)attr->name, "x") == 0) {  // if attribute is x
                    // set the x to the attributres contents, and parse the unit to the newRects unit if there are any
                    newRect->x = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "y") == 0) {  // if attribute is y
                    // set the y to the attributres contents, and parse the unit to the newRects unit if there are any
                    newRect->y = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "width") == 0) {  // if attribute is width
                    // set the width to the attributres contents, and parse the unit to the newRects unit if there are any
                    newRect->width = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "height") == 0) {  // if attribute is height
                    // set the height to the attributres contents, and parse the unit to the newRects unit if there are any
                    newRect->height = removeUnits((char*)value->content, newRect->units);
                }
                else {                                                           // if it is another attribute
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(attribute));  // create new attribute
                    strcpy(tmpAttr->name, (char*)attr->name);                    // set the name to the attributes name
                    strcpy(tmpAttr->value, (char*)value->content);               // set the content to the attributes content
                    insertBack(newRect->otherAttributes, tmpAttr);               // insert the attribute to the back of the list
                }
            }

            if (currGroup == NULL) {  // if there is no group, save the new rect to the SVG
                insertBack(currSVG->rectangles, newRect);
            }
            else {  // if there is a group, save the new rect to the group
                insertBack(currGroup->rectangles, newRect);
            }
        }
        else if (strcmp((char*)currNode->name, "circle") == 0) {
            xmlAttr* attr;
            Circle* newCircle = malloc(sizeof(Circle));
            newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
            for (attr = currNode->properties; attr != NULL; attr = attr->next) {
                xmlNode* value = attr->children;

                if (strcmp((char*)attr->name, "cx") == 0) {
                    newCircle->cx = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "cy") == 0) {
                    newCircle->cy = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "r") == 0) {
                    newCircle->r = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "units") == 0) {
                    strcpy(newCircle->units, (char*)value->content);
                }
                else {
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(attribute));
                    strcpy(tmpAttr->name, (char*)attr->name);
                    strcpy(tmpAttr->value, (char*)value->content);
                    insertBack(newCircle->otherAttributes, tmpAttr);
                }
            }

            if (currGroup == NULL) {
                insertBack(currSVG->circles, newCircle);
            }
            else {
                insertBack(currGroup->circles, newCircle);
            }
        }
        else if (strcmp((char*)currNode->name, "path") == 0) {
            xmlAttr* attr;
            Path* newPath = malloc(sizeof(Path));
            newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
            for (attr = currNode->properties; attr != NULL; attr = attr->next) {
                xmlNode* value = attr->children;

                if (strcmp((char*)attr->name, "data") == 0) {
                    strcpy(newPath->data, (char*)value->content);
                }
                else {
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(attribute));
                    strcpy(tmpAttr->name, (char*)attr->name);
                    strcpy(tmpAttr->value, (char*)value->content);
                    insertBack(newPath->otherAttributes, tmpAttr);
                }
            }

            if (currGroup == NULL) {
                insertBack(currSVG->paths, newPath);
            }
            else {
                insertBack(currGroup->paths, newPath);
            }
        }
        else if (strcmp((char*)currNode->name, "g") == 0) {
            Group* newGroup = createNewGroup();
            parseSVG(currNode->children, NULL, newGroup);

            if (currGroup == NULL) {
                insertBack(currSVG->groups, newGroup);
            }
            else {
                insertBack(currGroup->groups, newGroup);
            }
        }
        parseSVG(currNode->children, currSVG, NULL);
    }
}

float removeUnits(char* string, char* unitsStr) {
    char otherString[strlen(string)];
    char numberString[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int numValChars = 0;
    int numUnitChars = 0;

    for (int i = 0; string[i] != '\0'; i++) {
        for (int j = 0; j < 10; j++) {
            if ((string[i] == numberString[j]) || (j == 0 && string[i] == '-')) {
                otherString[numValChars] = string[i];
                numValChars++;
                break;
            }
            else if (strlen(unitsStr) == 0) {
                unitsStr[numUnitChars] = string[i];
                numUnitChars++;
                break;
            }
        }
    }

    return (atof(otherString));
}

Group* createNewGroup() {
    Group* newGroup = malloc(sizeof(Group));
    newGroup->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);       // create empty rect list
    newGroup->circles = initializeList(circleToString, deleteCircle, compareCircles);                   // create empty circle list
    newGroup->paths = initializeList(pathToString, deletePath, comparePaths);                           // create empty path list
    newGroup->groups = initializeList(groupToString, deleteGroup, compareGroups);                       // create empty group list
    newGroup->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list
    return newGroup;
}
