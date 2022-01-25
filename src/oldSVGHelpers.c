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
            Rectangle* newRect = (Rectangle*)malloc(sizeof(Rectangle));                                        // create a new rect struct.
            newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initalize the attribute list

            printf("pre loop\n");

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // create another node to get the content easier
                printf("Loop\n");
                if (strcmp((char*)attr->name, "x") == 0) {  // if attribute is x
                    // set the x to the attributres contents, and parse the unit to the newRects unit if there are any
                    printf("Loop1\n");
                    newRect->x = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "y") == 0) {  // if attribute is y
                    // set the y to the attributres contents, and parse the unit to the newRects unit if there are any
                    printf("Loop2\n");
                    newRect->y = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "width") == 0) {  // if attribute is width
                    // set the width to the attributres contents, and parse the unit to the newRects unit if there are any
                    printf("Loop3\n");
                    newRect->width = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "height") == 0) {  // if attribute is height
                    // set the height to the attributres contents, and parse the unit to the newRects unit if there are any
                    printf("Loop4\n");
                    newRect->height = removeUnits((char*)value->content, newRect->units);
                }
                else {
                    printf("Loop5\n");
                    printf("%s|%s\n", (char*)attr->name, (char*)value->content);

                    // if it is another attribute
                    printf("add to list0\n");
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute) + strlen((char*)value->content) + 1);  // create new attribute
                    printf("add to list1\n");
                    strcpy(tmpAttr->name, (char*)attr->name);  // set the name to the attributes name
                    printf("add to list2\n");
                    strcpy(tmpAttr->value, (char*)value->content);  // set the content to the attributes content
                    printf("add to list3\n");
                    insertBack(newRect->otherAttributes, tmpAttr);  // insert the attribute to the back of the list
                }
                printf("End Loop\n");
            }

            printf("save rect\n");
            if (currGroup == NULL) {  // if there is no group, save the new rect to the SVG
                insertBack(currSVG->rectangles, newRect);
            }
            else {  // if there is a group, save the new rect to the group
                insertBack(currGroup->rectangles, newRect);
            }
        }
        // if the current node is a circle
        else if (strcmp((char*)currNode->name, "circle") == 0) {
            xmlAttr* attr;                                                                                       // create new attr to store all the attributes
            Circle* newCircle = (Circle*)malloc(sizeof(Circle));                                                 // create a new circle
            newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initalize the attribute list

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop though all attributes
                xmlNode* value = attr->children;                                  // set value to the child for easier access to content

                if (strcmp((char*)attr->name, "cx") == 0) {  // if the attribute is cx
                    // set the cx to the attributes content, and parse the units to the newCircles units if there are any
                    newCircle->cx = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "cy") == 0) {  // if the attribute is cy
                    // set the cy to the attributes content, and parse the units to the newCircles units if there are any
                    newCircle->cy = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "r") == 0) {  // if the attribute is r
                    // set the r to the attributes content, and parse the units to the newCircles units if there are any
                    newCircle->r = removeUnits((char*)value->content, newCircle->units);
                }
                else {                                                           // if it is another attribute
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute));  // create new attribute
                    strcpy(tmpAttr->name, (char*)attr->name);                    // set the name to the attributes name
                    strcpy(tmpAttr->value, (char*)value->content);               // set the content to the attributes content
                    insertBack(newCircle->otherAttributes, tmpAttr);             // insert the attribute to the back of the list
                }
            }

            if (currGroup == NULL) {  // if there is no group, save the new circle to the svg
                insertBack(currSVG->circles, newCircle);
            }
            else {  // if there is a group, save the new circle to the group
                insertBack(currGroup->circles, newCircle);
            }
        }
        // if the current node is a path
        else if (strcmp((char*)currNode->name, "path") == 0) {
            xmlAttr* attr;                                                                                     // create a new attribute to manipulate the other attributes
            Path* newPath = (Path*)malloc(sizeof(Path));                                                       // create a new path
            newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initalize the attributes list

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // set value to the child for easier access to the content

                if (strcmp((char*)attr->name, "data") == 0) {      // if the attribute is data
                    strcpy(newPath->data, (char*)value->content);  // set the new paths data to the attribututes data.
                }
                else {                                                           // if it is another attribute
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute));  // create new attribute
                    strcpy(tmpAttr->name, (char*)attr->name);                    // set the name to the attributes name
                    strcpy(tmpAttr->value, (char*)value->content);               // set the content to the attributes content
                    insertBack(newPath->otherAttributes, tmpAttr);               // insert the attribute to the back of the list
                }
            }

            if (currGroup == NULL) {  // if there is no group, save the new path to the svg
                insertBack(currSVG->paths, newPath);
            }
            else {  // if there is a group, save the new path to the group
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
    char otherString[50];
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
    Group* newGroup = (Group*)malloc(sizeof(Group));
    newGroup->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);       // create empty rect list
    newGroup->circles = initializeList(circleToString, deleteCircle, compareCircles);                   // create empty circle list
    newGroup->paths = initializeList(pathToString, deletePath, comparePaths);                           // create empty path list
    newGroup->groups = initializeList(groupToString, deleteGroup, compareGroups);                       // create empty group list
    newGroup->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list
    return newGroup;
}
