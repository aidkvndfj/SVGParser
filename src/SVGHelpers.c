#include "SVGHelpers.h"

#include "SVGParser.h"

void parseSVG(xmlNode* rootNode, SVG* currSVG, Group* currGroup) {
    xmlNode* currNode = NULL;

    for (currNode = rootNode; currNode != NULL; currNode = currNode->next) {
        if (strcmp((char*)currNode->name, "svg") == 0) {
            xmlAttr* attr;  // create a new attribute to manipulate the other attributes

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // set value to the child for easier access to the content

                Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute) + strlen((char*)value->content) + 1);  // create new attribute
                tmpAttr->name = malloc(strlen((char*)attr->name) + 1);                                           // malloc space for the name
                strcpy(tmpAttr->name, (char*)attr->name);                                                        // set the name to the attributes name
                strcpy(tmpAttr->value, (char*)value->content);                                                   // set the content to the attributes content
                insertBack(currSVG->otherAttributes, tmpAttr);                                                   // insert the attribute to the back of the list
            }
        }
        // if the node is the title, set the SVG title to the childrens content, aka the title content.
        if (strcmp((char*)currNode->name, "title") == 0) {
            strcpy(currSVG->title, (char*)currNode->children->content);
        }
        // if the node is the description, set the SVG dec. to the childrens content, aka the desc. content.
        else if (strcmp((char*)currNode->name, "desc") == 0) {
            strcpy(currSVG->description, (char*)currNode->children->content);
        }
        else if (strcmp((char*)currNode->name, "rect") == 0) {
            xmlAttr* attr;                                                                                     // create attr variable to keep track of the attributes
            Rectangle* newRect = (Rectangle*)malloc(sizeof(Rectangle));                                        // create a new rect struct.
            newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initalize the attribute list
            strcpy(newRect->units, "");

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // create another node to get the content easier

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
                else {
                    // if it is another attribute
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute) + strlen((char*)value->content) + 1);  // create new attribute
                    tmpAttr->name = malloc(strlen((char*)attr->name) + 1);
                    strcpy(tmpAttr->name, (char*)attr->name);       // set the name to the attributes name
                    strcpy(tmpAttr->value, (char*)value->content);  // set the content to the attributes content
                    insertBack(newRect->otherAttributes, tmpAttr);  // insert the attribute to the back of the list
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
            xmlAttr* attr;                                                                                       // create attr variable to keep track of the attributes
            Circle* newCircle = (Circle*)malloc(sizeof(Circle));                                                 // create a new rect struct.
            newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initalize the attribute list
            strcpy(newCircle->units, "");

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // create another node to get the content easier

                if (strcmp((char*)attr->name, "cx") == 0) {  // if attribute is cx
                    // set the cx to the attributres contents, and parse the unit to the newCircles unit if there are any
                    newCircle->cx = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "cy") == 0) {  // if attribute is cy
                    // set the cy to the attributres contents, and parse the unit to the newCircles unit if there are any
                    newCircle->cy = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "r") == 0) {  // if attribute is r
                    // set the r to the attributres contents, and parse the unit to the newCircles unit if there are any
                    newCircle->r = removeUnits((char*)value->content, newCircle->units);
                }
                else {
                    // if it is another attribute
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute) + strlen((char*)value->content) + 1);  // create new attribute
                    tmpAttr->name = malloc(strlen((char*)attr->name) + 1);
                    strcpy(tmpAttr->name, (char*)attr->name);         // set the name to the attributes name
                    strcpy(tmpAttr->value, (char*)value->content);    // set the content to the attributes content
                    insertBack(newCircle->otherAttributes, tmpAttr);  // insert the attribute to the back of the list
                }
            }

            if (currGroup == NULL) {  // if there is no group, save the new circle to the SVG
                insertBack(currSVG->circles, newCircle);
            }
            else {  // if there is a group, save the new circle to the group
                insertBack(currGroup->circles, newCircle);
            }
        }
        else if (strcmp((char*)currNode->name, "path") == 0) {
            xmlAttr* attr;                                                                                     // create a new attribute to manipulate the other attributes
            Path* newPath = (Path*)malloc(sizeof(Path));                                                       // create a new path
            newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initalize the attributes list

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // set value to the child for easier access to the content

                if (strcmp((char*)attr->name, "d") == 0) {  // if the attribute is data
                    newPath = realloc(newPath, (sizeof(Path) + strlen((char*)value->content) + 1));
                    strcpy(newPath->data, (char*)value->content);  // set the new paths data to the attribututes data.
                }
                else {                                                                                               // if it is another attribute
                    Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute) + strlen((char*)value->content) + 1);  // create new attribute
                    tmpAttr->name = malloc(strlen((char*)attr->name) + 1);                                           // malloc space for the name
                    strcpy(tmpAttr->name, (char*)attr->name);                                                        // set the name to the attributes name
                    strcpy(tmpAttr->value, (char*)value->content);                                                   // set the content to the attributes content
                    insertBack(newPath->otherAttributes, tmpAttr);                                                   // insert the attribute to the back of the list
                }
            }

            if (currGroup == NULL) {  // if there is no group, save the new path to the svg
                insertBack(currSVG->paths, newPath);
            }
            else {  // if there is a group, save the new path to the group
                insertBack(currGroup->paths, newPath);
            }
        }
        if (strcmp((char*)currNode->name, "g") == 0) {
            Group* newGroup = createNewGroup();
            parseSVG(currNode->children, NULL, newGroup);

            xmlAttr* attr;
            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // set value to the child for easier access to the content

                Attribute* tmpAttr = (Attribute*)malloc(sizeof(Attribute) + strlen((char*)value->content) + 1);  // create new attribute
                tmpAttr->name = malloc(strlen((char*)attr->name) + 1);                                           // malloc space for the name
                strcpy(tmpAttr->name, (char*)attr->name);                                                        // set the name to the attributes name
                strcpy(tmpAttr->value, (char*)value->content);                                                   // set the content to the attributes content
                insertBack(newGroup->otherAttributes, tmpAttr);                                                  // insert the attribute to the back of the list
            }

            if (currGroup == NULL) {
                insertBack(currSVG->groups, newGroup);
            }
            else {
                insertBack(currGroup->groups, newGroup);
            }
        }
        else {
            parseSVG(currNode->children, currSVG, NULL);
        }
    }
}

float removeUnits(char* string, char* unitsStr) {
    char otherString[50] = "";
    char numberString[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
    int numValChars = 0;
    int numUnitChars = 0;
    int setUnits = strlen(unitsStr) == 0;

    for (int i = 0; string[i] != '\0'; i++) {
        for (int j = 0; j < 11; j++) {
            if ((string[i] == numberString[j]) || (j == 0 && string[i] == '-')) {
                otherString[numValChars] = string[i];
                numValChars++;
                break;
            }
            else if (setUnits && j == 10) {
                unitsStr[numUnitChars] = string[i];
                numUnitChars++;
                break;
            }
        }
    }

    otherString[numValChars] = '\0';

    if (setUnits)
        unitsStr[numUnitChars] = '\0';

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

// Recursively go through the group, and add all rects to rectList
void addGroupRects(List* rectList, Group* group) {
    void* elem;
    Rectangle* currRect;
    Group* currGroup;

    ListIterator rectIter = createIterator(group->rectangles);
    ListIterator groupIter = createIterator(group->groups);

    // loop through the rectList, and add the current rect to the total rects list
    while ((elem = nextElement(&rectIter)) != NULL) {
        currRect = (Rectangle*)elem;
        insertBack(rectList, currRect);
    }

    // loop through all the groups and add their rects to the total rects list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        addGroupRects(rectList, currGroup);
    }
}

// Recursively go through the group, and add all circles to circleList
void addGroupCircles(List* circleList, Group* group) {
    void* elem;
    Circle* currCircle;
    Group* currGroup;

    ListIterator circleIter = createIterator(group->circles);
    ListIterator groupIter = createIterator(group->groups);

    // loop through the circleList, and add the current circle to the total circles list
    while ((elem = nextElement(&circleIter)) != NULL) {
        currCircle = (Circle*)elem;
        insertBack(circleList, currCircle);
    }

    // loop through all the groups and add their circles to the total circles list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        addGroupCircles(circleList, currGroup);
    }
}

// Recursively go through the group, and add all paths to pathList
void addGroupPaths(List* pathList, Group* group) {
    void* elem;
    Path* currPath;
    Group* currGroup;

    ListIterator PathIter = createIterator(group->paths);
    ListIterator groupIter = createIterator(group->groups);

    // loop through the pathList, and add the current Path to the total Paths list
    while ((elem = nextElement(&PathIter)) != NULL) {
        currPath = (Path*)elem;
        insertBack(pathList, currPath);
    }

    // loop through all the groups and add their Paths to the total Paths list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        addGroupPaths(pathList, currGroup);
    }
}

// Recursively go through the group, and add all groups to groupList
void addGroupGroups(List* groupList, Group* group) {
    void* elem;
    Group* currGroup;

    ListIterator groupIter = createIterator(group->groups);

    // loop through all the groups and add their Paths to the total Paths list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        insertBack(groupList, currGroup);
        addGroupGroups(groupList, currGroup);
    }
}

// count the total number of other attributes in a list of rects
int numAttrInRectList(List* rectList) {
    void* elem;
    Rectangle* currRectangle;

    ListIterator groupIter = createIterator(rectList);
    int totalAttributes = 0;

    // loop through all the Rects and add their other attributes to total attributes
    while ((elem = nextElement(&groupIter)) != NULL) {
        currRectangle = (Rectangle*)elem;
        totalAttributes += getLength(currRectangle->otherAttributes);
    }

    return totalAttributes;
}

// count the total number of other attributes in a list of circless
int numAttrInCircleList(List* circleList) {
    void* elem;
    Circle* currCircle;

    ListIterator groupIter = createIterator(circleList);
    int totalAttributes = 0;

    // loop through all the Rects and add their other attributes to total attributes
    while ((elem = nextElement(&groupIter)) != NULL) {
        currCircle = (Circle*)elem;
        totalAttributes += getLength(currCircle->otherAttributes);
    }

    return totalAttributes;
}

// count the total number of other attributes in a list of paths
int numAttrInPathList(List* pathList) {
    void* elem;
    Path* currPath;

    ListIterator groupIter = createIterator(pathList);
    int totalAttributes = 0;

    // loop through all the Rects and add their other attributes to total attributes
    while ((elem = nextElement(&groupIter)) != NULL) {
        currPath = (Path*)elem;
        totalAttributes += getLength(currPath->otherAttributes);
    }

    return totalAttributes;
}

// count the total number of other attributes in a list of groups
int numAttrInGroupList(List* groupList) {
    void* elem;
    Group* currGroup;

    ListIterator groupIter = createIterator(groupList);
    int totalAttributes = 0;

    // loop through all the Rects and add their other attributes to total attributes
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        totalAttributes += getLength(currGroup->otherAttributes);
    }

    return totalAttributes;
    return 0;
}

void doNothing(void* data) {
    return;
}

//~~~~~~~ A2 Helpers ~~~~~~~~//
xmlNodePtr createTree(const SVG* currSVG, Group* bGroup, xmlNodePtr rootNode) {
    xmlNodePtr tempNode;

    Rectangle* currRect;
    Circle* currCircle;
    Path* currPath;
    Group* currGroup;
    Attribute* currAttr;
    ListIterator elementIterator;
    ListIterator attrIterator;
    void* elem;
    void* elem2;

    char buff[256];

    // make sure no lists are null
    if (currSVG != NULL) {
        if (currSVG->rectangles == NULL || currSVG->circles == NULL || currSVG->paths == NULL || currSVG->groups == NULL || currSVG->otherAttributes == NULL)
            return NULL;
    }
    else if (bGroup != NULL) {
        if (bGroup->rectangles == NULL || bGroup->circles == NULL || bGroup->paths == NULL || bGroup->groups == NULL || bGroup->otherAttributes == NULL)
            return NULL;
    }
    else {
        return NULL;
    }

    if (currSVG != NULL) {
        rootNode = xmlNewNode(NULL, BAD_CAST "svg");

        xmlNsPtr newNS = xmlNewNs(rootNode, (xmlChar*)currSVG->namespace, NULL);
        xmlSetNs(rootNode, newNS);

        attrIterator = createIterator(currSVG->otherAttributes);
        for (elem2 = nextElement(&attrIterator); elem2 != NULL; elem2 = nextElement(&attrIterator)) {
            currAttr = (Attribute*)elem2;

            if (currAttr->name == NULL)  // make sure the attr name isn't NULL
                return NULL;

            xmlNewProp(rootNode, BAD_CAST currAttr->name, BAD_CAST currAttr->value);
        }

        if (strcmp(currSVG->title, "") != 0)
            xmlNewChild(rootNode, NULL, BAD_CAST "title", BAD_CAST currSVG->title);

        if (strcmp(currSVG->description, "") != 0)
            xmlNewChild(rootNode, NULL, BAD_CAST "desc", BAD_CAST currSVG->description);
    }

    // add all rects to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->rectangles) : createIterator(bGroup->rectangles);
    for (elem = nextElement(&elementIterator); elem != NULL; elem = nextElement(&elementIterator)) {
        currRect = (Rectangle*)elem;
        tempNode = xmlNewChild(rootNode, NULL, BAD_CAST "rect", NULL);

        // make sure other attributes isn't null
        if (currRect->otherAttributes == NULL)
            return NULL;

        sprintf(buff, "%f%s", currRect->x, currRect->units);
        xmlNewProp(tempNode, BAD_CAST "x", BAD_CAST buff);

        sprintf(buff, "%f%s", currRect->y, currRect->units);
        xmlNewProp(tempNode, BAD_CAST "y", BAD_CAST buff);

        sprintf(buff, "%f%s", currRect->width, currRect->units);
        xmlNewProp(tempNode, BAD_CAST "width", BAD_CAST buff);

        sprintf(buff, "%f%s", currRect->height, currRect->units);
        xmlNewProp(tempNode, BAD_CAST "height", BAD_CAST buff);

        attrIterator = createIterator(currRect->otherAttributes);
        for (elem2 = nextElement(&attrIterator); elem2 != NULL; elem2 = nextElement(&attrIterator)) {
            currAttr = (Attribute*)elem2;

            if (currAttr->name == NULL)  // make sure the attr name isn't NULL
                return NULL;

            xmlNewProp(tempNode, BAD_CAST currAttr->name, BAD_CAST currAttr->value);
        }
    }

    // add all rects to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->circles) : createIterator(bGroup->circles);
    for (elem = nextElement(&elementIterator); elem != NULL; elem = nextElement(&elementIterator)) {
        currCircle = (Circle*)elem;
        tempNode = xmlNewChild(rootNode, NULL, BAD_CAST "circle", NULL);

        if (currCircle->otherAttributes == NULL)
            return NULL;

        // if (strcmp(currCircle->units, "") == 0)
        sprintf(buff, "%f%s", currCircle->cx, currCircle->units);
        // else
        // sprintf(buff, "%f", currCircle->x);
        xmlNewProp(tempNode, BAD_CAST "cx", BAD_CAST buff);

        // if (strcmp(currCircle->units, "") == 0)
        sprintf(buff, "%f%s", currCircle->cy, currCircle->units);
        // else
        // sprintf(buff, "%f", currCircle->y);
        xmlNewProp(tempNode, BAD_CAST "cy", BAD_CAST buff);

        // if (strcmp(currCircle->units, "") == 0)
        sprintf(buff, "%f%s", currCircle->r, currCircle->units);
        // else
        // sprintf(buff, "%f", currCircle->width);
        xmlNewProp(tempNode, BAD_CAST "r", BAD_CAST buff);

        attrIterator = createIterator(currCircle->otherAttributes);
        for (elem2 = nextElement(&attrIterator); elem2 != NULL; elem2 = nextElement(&attrIterator)) {
            currAttr = (Attribute*)elem2;

            if (currAttr->name == NULL)  // make sure the attr name isn't NULL
                return NULL;

            xmlNewProp(tempNode, BAD_CAST currAttr->name, BAD_CAST currAttr->value);
        }
    }

    // add all rects to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->paths) : createIterator(bGroup->paths);
    for (elem = nextElement(&elementIterator); elem != NULL; elem = nextElement(&elementIterator)) {
        currPath = (Path*)elem;
        tempNode = xmlNewChild(rootNode, NULL, BAD_CAST "path", NULL);

        // make sure that the data and other attributes aren't NULL
        if (currPath->otherAttributes == NULL || currPath->data == NULL)
            return NULL;

        xmlNewProp(tempNode, BAD_CAST "d", BAD_CAST currPath->data);

        attrIterator = createIterator(currPath->otherAttributes);
        for (elem2 = nextElement(&attrIterator); elem2 != NULL; elem2 = nextElement(&attrIterator)) {
            currAttr = (Attribute*)elem2;

            if (currAttr->name == NULL)  // make sure the attr name isn't NULL
                return NULL;

            xmlNewProp(tempNode, BAD_CAST currAttr->name, BAD_CAST currAttr->value);
        }
    }

    // add all rects to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->groups) : createIterator(bGroup->groups);
    for (elem = nextElement(&elementIterator); elem != NULL; elem = nextElement(&elementIterator)) {
        currGroup = (Group*)elem;

        tempNode = xmlNewChild(rootNode, NULL, BAD_CAST "g", NULL);
        createTree(NULL, currGroup, tempNode);

        if (tempNode == NULL)
            return NULL;

        attrIterator = createIterator(currGroup->otherAttributes);
        for (elem2 = nextElement(&attrIterator); elem2 != NULL; elem2 = nextElement(&attrIterator)) {
            currAttr = (Attribute*)elem2;

            if (currAttr->name == NULL)  // make sure the attr name isn't NULL
                return NULL;

            xmlNewProp(tempNode, BAD_CAST currAttr->name, BAD_CAST currAttr->value);
        }

        xmlAddChild(rootNode, tempNode);
    }

    return rootNode;
}

bool validateDoc(xmlDocPtr doc, const char* schemaFile) {
    /** parts of this code have been taken from
     * http://knol2share.blogspot.com/2009/05/validate-xml-against-xsd-in-c.html
     * and modified for use in this project */

    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr parserCtxt;

    int ret;

    xmlLineNumbersDefault(1);

    parserCtxt = xmlSchemaNewParserCtxt(schemaFile);

    xmlSchemaSetParserErrors(parserCtxt, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
    schema = xmlSchemaParse(parserCtxt);
    xmlSchemaFreeParserCtxt(parserCtxt);

    if (doc == NULL)
        return false;

    if (doc == NULL) {
        // free the schema
        if (schema != NULL)
            xmlSchemaFree(schema);

        xmlSchemaCleanupTypes();
        xmlCleanupParser();

        perror("Couldn't parse the SVG in createValidateSVG");
        return false;
    }

    xmlSchemaValidCtxtPtr validCtxt;
    validCtxt = xmlSchemaNewValidCtxt(schema);
    xmlSchemaSetValidErrors(validCtxt, (xmlSchemaValidityErrorFunc)fprintf, (xmlSchemaValidityWarningFunc)fprintf, stderr);
    ret = xmlSchemaValidateDoc(validCtxt, doc);
    xmlSchemaFreeValidCtxt(validCtxt);
    xmlFreeDoc(doc);

    // free the schema
    if (schema != NULL)
        xmlSchemaFree(schema);

    xmlSchemaCleanupTypes();
    xmlCleanupParser();

    if (ret == 0) {
        return true;
    }
    else if (ret > 0) {
        printf("SVG fails to validate, create\n");
    }
    else {
        printf("SVG validation generated an internal error, create\n");
    }
    return false;
}

bool validNumber(char* string) {
    char valid[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    bool hasDot = false;
    bool matchFlag = false;

    for (int i = 0; i < strlen(string); i++) {
        matchFlag = false;

        if (string[i] == '.') {
            if (hasDot) {
                return false;
            }
            else {
                hasDot = true;
            }
        }
        else {
            for (int j = 0; j < 10; j++) {
                if (string[i] == valid[j]) {
                    matchFlag = true;
                    break;
                }
            }

            if (!matchFlag) {
                return false;
            }
        }
    }

    return true;
}
