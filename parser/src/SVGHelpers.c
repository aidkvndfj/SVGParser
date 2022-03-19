#include "SVGHelpers.h"

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
            newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initialize the attribute list
            strcpy(newRect->units, "");

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // create another node to get the content easier

                if (strcmp((char*)attr->name, "x") == 0) {  // if attribute is x
                    // set the x to the attributes contents, and parse the unit to the newRects unit if there are any
                    newRect->x = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "y") == 0) {  // if attribute is y
                    // set the y to the attributes contents, and parse the unit to the newRects unit if there are any
                    newRect->y = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "width") == 0) {  // if attribute is width
                    // set the width to the attributes contents, and parse the unit to the newRects unit if there are any
                    newRect->width = removeUnits((char*)value->content, newRect->units);
                }
                else if (strcmp((char*)attr->name, "height") == 0) {  // if attribute is height
                    // set the height to the attributes contents, and parse the unit to the newRects unit if there are any
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
            newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initialize the attribute list
            strcpy(newCircle->units, "");

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // create another node to get the content easier

                if (strcmp((char*)attr->name, "cx") == 0) {  // if attribute is cx
                    // set the cx to the attributes contents, and parse the unit to the newCircles unit if there are any
                    newCircle->cx = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "cy") == 0) {  // if attribute is cy
                    // set the cy to the attributes contents, and parse the unit to the newCircles unit if there are any
                    newCircle->cy = removeUnits((char*)value->content, newCircle->units);
                }
                else if (strcmp((char*)attr->name, "r") == 0) {  // if attribute is r
                    // set the r to the attributes contents, and parse the unit to the newCircles unit if there are any
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
            newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // initialize the attributes list

            for (attr = currNode->properties; attr != NULL; attr = attr->next) {  // loop through all the attributes
                xmlNode* value = attr->children;                                  // set value to the child for easier access to the content

                if (strcmp((char*)attr->name, "d") == 0) {  // if the attribute is data
                    newPath = realloc(newPath, (sizeof(Path) + strlen((char*)value->content) + 1));
                    strcpy(newPath->data, (char*)value->content);  // set the new paths data to the attributes data.
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

    // loop until you reach null char
    for (int i = 0; string[i] != '\0'; i++) {
        for (int j = 0; j < 11; j++) {                                             // loop through the valid chars list
            if ((string[i] == numberString[j]) || (j == 0 && string[i] == '-')) {  // if the current char is valid or a - is the first char
                otherString[numValChars] = string[i];
                numValChars++;
                break;
            }
            else if (setUnits && j == 10) {  // if we need to set the units, and we're looking at the last of the valid chars
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

    // only do this if it's the first pass, and we are looking at the SVG
    if (currSVG != NULL) {
        rootNode = xmlNewNode(NULL, BAD_CAST "svg");  // create the root node

        // set namespace
        xmlNsPtr newNS = xmlNewNs(rootNode, (xmlChar*)currSVG->namespace, NULL);
        xmlSetNs(rootNode, newNS);

        // loop through SVG other attributes and add them to the tree
        attrIterator = createIterator(currSVG->otherAttributes);
        for (elem2 = nextElement(&attrIterator); elem2 != NULL; elem2 = nextElement(&attrIterator)) {
            currAttr = (Attribute*)elem2;

            if (currAttr->name == NULL)  // make sure the attr name isn't NULL
                return NULL;

            xmlNewProp(rootNode, BAD_CAST currAttr->name, BAD_CAST currAttr->value);
        }

        // if title isn't empty, add it as a node
        if (strcmp(currSVG->title, "") != 0)
            xmlNewChild(rootNode, NULL, BAD_CAST "title", BAD_CAST currSVG->title);

        // if desc isn't empty, add it as a node
        if (strcmp(currSVG->description, "") != 0)
            xmlNewChild(rootNode, NULL, BAD_CAST "desc", BAD_CAST currSVG->description);
    }

    // add all rects to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->rectangles) : createIterator(bGroup->rectangles);  // check if we're iterating through the svg, or through a group
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

    // add all circles to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->circles) : createIterator(bGroup->circles);  // check if we're iterating through the svg, or through a group
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

    // add all paths to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->paths) : createIterator(bGroup->paths);  // check if we're iterating through the svg, or through a group
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

    // add all groups to the xml tree
    elementIterator = currSVG != NULL ? createIterator(currSVG->groups) : createIterator(bGroup->groups);  // check if we're iterating through the svg, or through a group
    for (elem = nextElement(&elementIterator); elem != NULL; elem = nextElement(&elementIterator)) {
        currGroup = (Group*)elem;

        // create a new node, and call createTree with tempNode as the root
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

    // return the root
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

int rectsInSVG(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    List* rects = getRects(currSVG);

    int num = getLength(rects);

    freeList(rects);
    deleteSVG(currSVG);

    return num;
}

int circsInSVG(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    List* circs = getCircles(currSVG);

    int num = getLength(circs);

    freeList(circs);
    deleteSVG(currSVG);

    return num;
}

int pathsInSVG(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    List* paths = getPaths(currSVG);

    int num = getLength(paths);

    freeList(paths);
    deleteSVG(currSVG);

    return num;
}

int groupsInSVG(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    List* groups = getGroups(currSVG);

    int num = getLength(groups);

    freeList(groups);
    deleteSVG(currSVG);

    return num;
}

int isValidSVG(char* fileName, char* schemaFile) {
    xmlDocPtr doc;
    doc = xmlReadFile(fileName, NULL, 0);  // read the file

    if (validateDoc(doc, schemaFile))
        return 1;

    return 0;
}

char* validSVGToJSON(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    if (currSVG != NULL) {
        char* tmpStr = SVGtoJSON(currSVG);
        deleteSVG(currSVG);
        return tmpStr;
    }
    else {
        return NULL;
    }
}

char* requestSVGTitle(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    char* title = currSVG->title;
    title = strtok(title, "\n");
    deleteSVG(currSVG);
    return title;
}

char* requestSVGDesc(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    char* desc = currSVG->description;
    desc = strtok(desc, "\n");
    deleteSVG(currSVG);
    return desc;
}

char* requestSVGRects(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    char* json = rectListToJSON(currSVG->rectangles);
    deleteSVG(currSVG);
    return json;
}

char* requestSVGCircles(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    char* json = circListToJSON(currSVG->circles);
    deleteSVG(currSVG);
    return json;
}

char* requestSVGPaths(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    char* json = pathListToJSON(currSVG->paths);
    deleteSVG(currSVG);
    return json;
}

char* requestSVGGroups(char* fileName, char* schemaFile) {
    SVG* currSVG = createValidSVG(fileName, schemaFile);
    char* json = groupListToJSON(currSVG->groups);
    deleteSVG(currSVG);
    return json;
}

char* requestIndexRectAttrs(char* fileName, int index) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->rectangles);
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }

    Rectangle* currRect = (Rectangle*)elem;

    char* json = attrListToJSON(currRect->otherAttributes);

    deleteSVG(currSVG);

    return json;
}

char* requestIndexCircAttrs(char* fileName, int index) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->circles);
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }

    Circle* currCircle = (Circle*)elem;

    char* json = attrListToJSON(currCircle->otherAttributes);

    deleteSVG(currSVG);

    return json;
}

char* requestIndexPathAttrs(char* fileName, int index) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->paths);
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }

    Path* currPath = (Path*)elem;

    char* json = attrListToJSON(currPath->otherAttributes);

    deleteSVG(currSVG);

    return json;
}

char* requestIndexGroupAttrs(char* fileName, int index) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->groups);
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }

    Group* currGroup = (Group*)elem;

    char* json = attrListToJSON(currGroup->otherAttributes);

    deleteSVG(currSVG);

    return json;
}

char* setRectAttribute(char* fileName, int index, char* attribute, char* val) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->rectangles);
    Attribute* currAttribute;
    Node* attrNode;
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }
    Rectangle* currRectangle = (Rectangle*)elem;

    if (strcmp(attribute, "x") == 0) {
        if (!validNumber(val))
            return "ERROR:INVALID_NUM";
        currRectangle->x = atof(val);
    }
    else if (strcmp(attribute, "y") == 0) {
        if (!validNumber(val))
            return "ERROR:INVALID_NUM";
        currRectangle->y = atof(val);
    }
    else if (strcmp(attribute, "width") == 0) {
        if (!validNumber(val))
            return "ERROR:INVALID_NUM";
        currRectangle->width = atof(val);
    }
    else if (strcmp(attribute, "height") == 0) {
        if (!validNumber(val))
            return "ERROR:INVALID_NUM";
        currRectangle->height = atof(val);
    }
    else {
        attrNode = currRectangle->otherAttributes->head;
        while (attrNode != NULL) {
            currAttribute = (Attribute*)attrNode->data;
            if (strcmp(currAttribute->name, attribute) == 0) {
                currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(val) + 1);
                strcpy(currAttribute->value, val);
                attrNode->data = currAttribute;
                break;
            }
            attrNode = attrNode->next;
        }
    }

    if (attrNode == NULL) {
        deleteSVG(currSVG);
        return "ERROR:NOT_FOUND";
    }

    if (validateSVG(currSVG, "svg.xsd") == false) {
        deleteSVG(currSVG);
        return "ERROR:INVALID_SVG";
    }
    writeSVG(currSVG, fileName);
    deleteSVG(currSVG);

    return "success";
}

char* setCircAttribute(char* fileName, int index, char* attribute, char* val) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->circles);
    Attribute* currAttribute;
    Node* attrNode;
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }
    Circle* currCircle = (Circle*)elem;

    if (strcmp(attribute, "cx") == 0) {
        if (!validNumber(val))
            return "ERROR:INVALID_NUM";
        currCircle->cx = atof(val);
    }
    else if (strcmp(attribute, "cy") == 0) {
        if (!validNumber(val))
            return "ERROR:INVALID_NUM";
        currCircle->cy = atof(val);
    }
    else if (strcmp(attribute, "r") == 0) {
        if (!validNumber(val))
            return "ERROR:INVALID_NUM";
        currCircle->r = atof(val);
    }
    else {
        attrNode = currCircle->otherAttributes->head;
        while (attrNode != NULL) {
            currAttribute = (Attribute*)attrNode->data;
            if (strcmp(currAttribute->name, attribute) == 0) {
                currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(val) + 1);
                strcpy(currAttribute->value, val);
                attrNode->data = currAttribute;
                break;
            }
            attrNode = attrNode->next;
        }
    }

    if (attrNode == NULL) {
        deleteSVG(currSVG);
        return "ERROR:NOT_FOUND";
    }

    if (validateSVG(currSVG, "svg.xsd") == false) {
        deleteSVG(currSVG);
        return "ERROR:INVALID_SVG";
    }
    writeSVG(currSVG, fileName);
    deleteSVG(currSVG);

    return "success";
}

char* setPathAttribute(char* fileName, int index, char* attribute, char* val) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->paths);
    Attribute* currAttribute;
    Node* attrNode;
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }
    Path* currPath = (Path*)elem;

    if (strcmp(attribute, "data") == 0) {
        currPath = realloc(currPath, sizeof(Path) + ((strlen(val) + 1) * sizeof(char)));
        strcpy(currPath->data, val);
    }
    else {
        attrNode = currPath->otherAttributes->head;
        while (attrNode != NULL) {
            currAttribute = (Attribute*)attrNode->data;
            if (strcmp(currAttribute->name, attribute) == 0) {
                currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(val) + 1);
                strcpy(currAttribute->value, val);
                attrNode->data = currAttribute;
                break;
            }
            attrNode = attrNode->next;
        }
    }

    if (attrNode == NULL) {
        deleteSVG(currSVG);
        return "ERROR:NOT_FOUND";
    }

    if (validateSVG(currSVG, "svg.xsd") == false) {
        deleteSVG(currSVG);
        return "ERROR:INVALID_SVG";
    }

    writeSVG(currSVG, fileName);
    deleteSVG(currSVG);

    return "success";
}

char* setGroupAttribute(char* fileName, int index, char* attribute, char* val) {
    SVG* currSVG = createSVG(fileName);
    ListIterator iter = createIterator(currSVG->groups);
    Attribute* currAttribute;
    Node* attrNode;
    void* elem;

    for (int i = 0; i < index; i++) {
        elem = nextElement(&iter);
    }
    Group* currGroup = (Group*)elem;

    attrNode = currGroup->otherAttributes->head;
    while (attrNode != NULL) {
        currAttribute = (Attribute*)attrNode->data;
        if (strcmp(currAttribute->name, attribute) == 0) {
            currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(val) + 1);
            strcpy(currAttribute->value, val);
            attrNode->data = currAttribute;
            break;
        }
        attrNode = attrNode->next;
    }

    if (attrNode == NULL) {
        deleteSVG(currSVG);
        return "ERROR:NOT_FOUND";
    }

    if (validateSVG(currSVG, "svg.xsd") == false) {
        deleteSVG(currSVG);
        return "ERROR:INVALID_SVG";
    }

    writeSVG(currSVG, fileName);
    deleteSVG(currSVG);
    return "success";
}

char* setSVGTitle(char* fileName, char* title) {
    SVG* currSVG = createSVG(fileName);
    strcpy(currSVG->title, title);
    if (validateSVG(currSVG, "svg.xsd")) {
        writeSVG(currSVG, fileName);
        free(currSVG);
        return "success";
    }

    free(currSVG);
    return "ERROR:INVALID_TITLE";
}

char* setSVGDesc(char* fileName, char* desc) {
    SVG* currSVG = createSVG(fileName);
    strcpy(currSVG->description, desc);

    if (validateSVG(currSVG, "svg.xsd")) {
        writeSVG(currSVG, fileName);
        free(currSVG);
        return "success";
    }

    free(currSVG);
    return "ERROR:INVALID_DESCRIPTION";
}

char* createNewSVG(char* fileName, int hasRect, int hasCirc, int hasPath) {
    SVG* newSVG = (SVG*)malloc(sizeof(SVG));
    strcpy(newSVG->namespace, "http://www.w3.org/2000/svg");
    strcpy(newSVG->title, "");
    strcpy(newSVG->description, "");

    newSVG->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);       // create empty rect list
    newSVG->circles = initializeList(circleToString, deleteCircle, compareCircles);                   // create empty circle list
    newSVG->paths = initializeList(pathToString, deletePath, comparePaths);                           // create empty path list
    newSVG->groups = initializeList(groupToString, deleteGroup, compareGroups);                       // create empty group list
    newSVG->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list

    if (hasRect == 1) {
        Rectangle* newRect = (Rectangle*)malloc(sizeof(Rectangle));
        newRect->x = 0;
        newRect->y = 0;
        newRect->width = 0;
        newRect->height = 0;
        strcpy(newRect->units, "");
        newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list
        insertBack(newSVG->rectangles, newRect);
    }

    if (hasCirc == 1) {
        Circle* newCircle = (Circle*)malloc(sizeof(Circle));
        newCircle->cx = 0;
        newCircle->cy = 0;
        newCircle->r = 0;
        strcpy(newCircle->units, "");
        newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list
        insertBack(newSVG->circles, newCircle);
    }

    if (hasPath == 1) {
        Path* newPath = (Path*)malloc(sizeof(Path) + 2);
        strcpy(newPath->data, "");
        newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list
        insertBack(newSVG->paths, newPath);
    }

    if (validateSVG(newSVG, "svg.xsd")) {
        writeSVG(newSVG, fileName);
        return "success";
    }
    else {
        return "ERROR:SVG_FAILED_CREATION";
    }
}

char* addRectangle(char* fileName, float xPos, float yPos, float width, float height) {
    SVG* currSVG = createSVG(fileName);

    Rectangle* newRect = (Rectangle*)malloc(sizeof(Rectangle));
    newRect->x = xPos;
    newRect->y = yPos;
    newRect->width = width;
    newRect->height = height;
    strcpy(newRect->units, "");
    newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list
    insertBack(currSVG->rectangles, newRect);

    if (validateSVG(currSVG, "svg.xsd")) {
        writeSVG(currSVG, fileName);
        free(currSVG);
        return "success";
    }
    else {
        free(currSVG);
        return "ERROR:INVALID_SVG_CREATED";
    }

    return "ERROR:UNKNOW_FAIL";
}

char* addCircle(char* fileName, float cxPos, float cyPos, float radius) {
    SVG* currSVG = createSVG(fileName);

    Circle* newCircle = (Circle*)malloc(sizeof(Circle));
    newCircle->cx = cxPos;
    newCircle->cy = cyPos;
    newCircle->r = radius;
    strcpy(newCircle->units, "");
    newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list
    insertBack(currSVG->circles, newCircle);

    if (validateSVG(currSVG, "svg.xsd")) {
        writeSVG(currSVG, fileName);
        free(currSVG);
        return "success";
    }
    else {
        free(currSVG);
        return "ERROR:INVALID_SVG_CREATED";
    }

    return "ERROR:UNKNOW_FAIL";
}
