#include "SVGParser.h"

#include "SVGHelpers.h"

//******************* Assignment 1 *******************//
//~~~~~~~~ Parser Functions ~~~~~~~~//
SVG* createSVG(const char* fileName) {
    SVG* newSVG = NULL;
    xmlDoc* doc = NULL;
    xmlNode* rootElement = NULL;

    newSVG = (SVG*)malloc(sizeof(SVG));    // malloc space for the SVG
    doc = xmlReadFile(fileName, NULL, 0);  // read the file

    // If the doc is null, the file couldn't be found
    if (doc == NULL) {
        fprintf(stderr, "Could not parse file: %s\n", fileName);
        return NULL;
    }

    rootElement = xmlDocGetRootElement(doc);  // get the root element

    // set the namespace, title, and desc. to empty string
    strcpy(newSVG->namespace, "");
    strcpy(newSVG->title, "");
    strcpy(newSVG->description, "");

    strcpy(newSVG->namespace, (char*)rootElement->ns->href);                                          // get the namespace
    newSVG->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);       // create empty rect list
    newSVG->circles = initializeList(circleToString, deleteCircle, compareCircles);                   // create empty circle list
    newSVG->paths = initializeList(pathToString, deletePath, comparePaths);                           // create empty path list
    newSVG->groups = initializeList(groupToString, deleteGroup, compareGroups);                       // create empty group list
    newSVG->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list

    // parse the file for all the SVG elements
    parseSVG(rootElement, newSVG, NULL);

    // free the things!
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return newSVG;
}

char* SVGToString(const SVG* img) {
    char* tmpStr;

    // get the 5 lists of the SVG as strings
    char* rectStr = toString(img->rectangles);
    char* circleStr = toString(img->circles);
    char* pathStr = toString(img->paths);
    char* groupStr = toString(img->groups);
    char* attrStr = toString(img->otherAttributes);

    // determine how many chars to alloc
    int len = strlen(rectStr) + strlen(circleStr) + strlen(pathStr) + strlen(groupStr) + strlen(attrStr) + strlen(img->namespace) + strlen(img->title) + strlen(img->description) + 140;
    tmpStr = (char*)malloc(sizeof(char) * len);  // malloc space for the tmp string

    // set the return string
    sprintf(tmpStr, "Namespace: %s\nTitle: %s\nDescription: %s\n\n", img->namespace, img->title, img->description);  // add namespace, title, and desc
    strcat(tmpStr, "\nSVG Other Attributes: \n");                                                                    // add the other attributes
    strcat(tmpStr, attrStr);
    strcat(tmpStr, "\nSVG Rectangles: \n");  // add the rectangle
    strcat(tmpStr, rectStr);
    strcat(tmpStr, "\nSVG Circles: \n");  // add the circles
    strcat(tmpStr, circleStr);
    strcat(tmpStr, "\nSVG Paths: \n");  // add the paths
    strcat(tmpStr, pathStr);
    strcat(tmpStr, "\nSVG Groups: \n");  // add the groups
    strcat(tmpStr, groupStr);

    // free the temp strings
    free(rectStr);
    free(circleStr);
    free(pathStr);
    free(groupStr);
    free(attrStr);

    return tmpStr;
}

void deleteSVG(SVG* img) {
    // if the img is null, nothing to free
    if (img == NULL)
        return;

    // free all the elements of the SVG as well as the SVG itself
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->paths);
    freeList(img->groups);
    freeList(img->otherAttributes);
    free(img);
}

//~~~~~~~~ Accessors ~~~~~~~~~//
// Function that returns a list of all rectangles in the struct.
List* getRects(const SVG* img) {
    if (img == NULL)
        return NULL;

    void* elem;
    Rectangle* currRect;
    Group* currGroup;

    List* totalRects = initializeList(rectangleToString, doNothing, compareRectangles);
    ListIterator rectIter = createIterator(img->rectangles);
    ListIterator groupIter = createIterator(img->groups);

    // loop through the rectanlges, and add the current rect to the total rects list
    while ((elem = nextElement(&rectIter)) != NULL) {
        currRect = (Rectangle*)elem;
        insertBack(totalRects, currRect);
    }

    // loop through all the groups and add their rects to the total rects list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        addGroupRects(totalRects, currGroup);
    }

    return totalRects;
}

// Function that returns a list of all circles in the struct.
List* getCircles(const SVG* img) {
    if (img == NULL)
        return NULL;

    void* elem;
    Circle* currCircle;
    Group* currGroup;

    List* totalCircles = initializeList(circleToString, doNothing, compareCircles);
    ListIterator circleIter = createIterator(img->circles);
    ListIterator groupIter = createIterator(img->groups);

    // loop through all the circles and add them to the total circles list
    while ((elem = nextElement(&circleIter)) != NULL) {
        currCircle = (Circle*)elem;
        insertBack(totalCircles, currCircle);
    }

    // loop through all the groups and add their circles to the total circles list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        addGroupCircles(totalCircles, currGroup);
    }

    return totalCircles;
}

// Function that returns a list of all groups in the struct.
List* getGroups(const SVG* img) {
    if (img == NULL)
        return NULL;

    void* elem;
    Group* currGroup;

    List* totalGroups = initializeList(groupToString, doNothing, compareGroups);
    ListIterator groupIter = createIterator(img->groups);

    // loop through all the groups and add them to the total group list, then add any groups inside the groups to the groups list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;

        insertBack(totalGroups, currGroup);
        addGroupGroups(totalGroups, currGroup);
    }

    return totalGroups;
}

// Function that returns a list of all paths in the struct.
List* getPaths(const SVG* img) {
    if (img == NULL)
        return NULL;

    void* elem;
    Path* currPath;
    Group* currGroup;

    List* totalPaths = initializeList(pathToString, doNothing, comparePaths);
    ListIterator pathIter = createIterator(img->paths);
    ListIterator groupIter = createIterator(img->groups);

    // loop through all the paths and add them to the total paths list
    while ((elem = nextElement(&pathIter)) != NULL) {
        currPath = (Path*)elem;
        insertBack(totalPaths, currPath);
    }

    // loop through all the groups and add their paths to the total paths list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;
        addGroupPaths(totalPaths, currGroup);
    }

    return totalPaths;
}

//~~~~~~~~ Summaries ~~~~~~~~~//
// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(const SVG* img, float area) {
    if (img == NULL || area < 0) {
        return 0;
    }

    Rectangle* currRect;
    void* elem;
    int numRects = 0;

    List* tmpRects = getRects(img);
    ListIterator iter = createIterator(tmpRects);

    // loop through all the rects, calculate the area, then check if it's the right area
    while ((elem = nextElement(&iter)) != NULL) {
        currRect = (Rectangle*)elem;
        if (ceil(currRect->width * currRect->height) == ceil(area)) {
            numRects += 1;
        }
    }

    // free the list and nodes of the tmp list, but keep the data in tact
    freeList(tmpRects);
    return numRects;
}

// Function that returns the number of all circles with the specified area
int numCirclesWithArea(const SVG* img, float area) {
    if (img == NULL || area < 0) {
        return 0;
    }

    Circle* currCircle;
    void* elem;
    int numCircle = 0;

    List* tmpCircle = getCircles(img);
    ListIterator iter = createIterator(tmpCircle);

    // loop through all the circles, calculate the area, then check if it's the right area
    while ((elem = nextElement(&iter)) != NULL) {
        currCircle = (Circle*)elem;
        if (ceil(PI * (currCircle->r * currCircle->r)) == ceil(area)) {
            numCircle += 1;
        }
    }

    // free the list and nodes of the tmp list, but keep the data in tact
    freeList(tmpCircle);
    return numCircle;
}

// Function that returns the number of all paths with the specified data
int numPathsWithdata(const SVG* img, const char* data) {
    if (img == NULL || data == NULL) {
        return 0;
    }

    Path* currPath;
    void* elem;
    int numPaths = 0;

    List* tmpPaths = getPaths(img);
    ListIterator iter = createIterator(tmpPaths);

    // loop through all the paths, and check if it has the correct data
    while ((elem = nextElement(&iter)) != NULL) {
        currPath = (Path*)elem;
        if (strcmp(currPath->data, data) == 0) {
            numPaths += 1;
        }
    }

    // free the list and nodes of the tmp list, but keep the data in tact
    freeList(tmpPaths);
    return numPaths;
}

// Function that returns the number of all groups with the specified length
int numGroupsWithLen(const SVG* img, int len) {
    if (img == NULL || len < 0) {
        return 0;
    }

    Group* currGroup;
    void* elem;
    int currLen;

    List* tmpGroups = getGroups(img);
    ListIterator iter = createIterator(tmpGroups);
    int numGroupsWithLen = 0;

    // loop through all the groups in the SVG
    while ((elem = nextElement(&iter)) != NULL) {
        currGroup = (Group*)elem;
        currLen = 0;  // set curr len to 0

        currLen += getLength(currGroup->rectangles);  // add the length of the rects list to currLen
        currLen += getLength(currGroup->circles);     // add the length of the circles list to currLen
        currLen += getLength(currGroup->paths);       // add the length of the paths list to currLen
        currLen += getLength(currGroup->groups);      // add the length of the groups list to currLen

        // if currLen is the right len, add 1 to total groups
        if (currLen == len) {
            numGroupsWithLen += 1;
        }
    }

    // free the list and nodes of the tmp list, but keep the data in tact
    freeList(tmpGroups);
    return numGroupsWithLen;
}

int numAttr(const SVG* img) {
    if (img == NULL) {
        return 0;
    }

    int currLen = 0;

    currLen += getLength(img->otherAttributes);

    List* tmpRect = getRects(img);      // get a list of the total rects in the SVG
    List* tmpCircle = getCircles(img);  // get a list of the total circles in the SVG
    List* tmpPath = getPaths(img);      // get a list of the total paths in the SVG
    List* tmpGroup = getGroups(img);    // get a list of the total groups in the SVG

    // add the total number of other attribtues in each list to the currLen
    currLen += numAttrInRectList(tmpRect);
    currLen += numAttrInCircleList(tmpCircle);
    currLen += numAttrInPathList(tmpPath);
    currLen += numAttrInGroupList(tmpGroup);

    // free the lists and nodes, but keep the data in-tact
    freeList(tmpRect);
    freeList(tmpCircle);
    freeList(tmpPath);
    freeList(tmpGroup);

    return currLen;
}

//******************* Assignment 2 *******************//
bool validateSVG(const SVG* img, const char* schemaFile) {
    xmlDocPtr doc;

    doc = xmlNewDoc(BAD_CAST "1.0");
    xmlDocSetRootElement(doc, createTree(img, NULL, NULL));

    return validateDoc(doc, schemaFile);
}

SVG* createValidSVG(const char* fileName, const char* schemaFile) {
    xmlDocPtr doc;
    doc = xmlReadFile(fileName, NULL, 0);  // read the file

    if (validateDoc(doc, schemaFile))
        return createSVG(fileName);

    return NULL;
}

bool writeSVG(const SVG* img, const char* fileName) {
    if (img == NULL)  // make sure img isn't null
        return false;

    xmlDocPtr newDoc;

    newDoc = xmlNewDoc(BAD_CAST "1.0");                         // create new doc
    xmlDocSetRootElement(newDoc, createTree(img, NULL, NULL));  // set the root of the doc to the node returned from createTree

    // if the newDoc is null, something failed, return false
    if (newDoc == NULL)
        return false;

    // save the doc to fileName
    xmlSaveFormatFileEnc(fileName, newDoc, "UTF-8", 1);

    // free the doc
    xmlFreeDoc(newDoc);
    xmlCleanupParser();

    return true;
}

bool setAttribute(SVG* img, elementType elemType, int elemIndex, Attribute* newAttribute) {
    ListIterator otherIterator;
    Attribute* currAttribute = NULL;
    Rectangle* currRect = NULL;
    Circle* currCircle = NULL;
    Path* currPath = NULL;
    Group* currGroup = NULL;
    bool freeFlag = true;
    void* elem;

    if (newAttribute == NULL || newAttribute->name == NULL || img == NULL || img->rectangles == NULL || img->circles == NULL || img->paths == NULL || img->groups == NULL || img->otherAttributes == NULL) {
        return false;
    }

    switch (elemType) {
        case SVG_IMG:
            if (strcmp(newAttribute->name, "xmlns") == 0) {  // check if new attribute is a namespace
                strcpy(img->namespace, newAttribute->value);
            }
            else if (strcmp(newAttribute->name, "title") == 0) {  // check if new attribute is a title
                strcpy(img->title, newAttribute->value);
            }
            else if (strcmp(newAttribute->name, "desc") == 0) {  // check if new attribute is a description
                strcpy(img->description, newAttribute->value);
            }
            else {
                // loop through the list of attributes and try to find a match
                Node* attrNode = img->otherAttributes->head;
                while (attrNode != NULL) {
                    currAttribute = (Attribute*)attrNode->data;
                    if (strcmp(currAttribute->name, newAttribute->name) == 0) {
                        currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(newAttribute->value) + 1);
                        strcpy(currAttribute->value, newAttribute->value);
                        attrNode->data = currAttribute;
                        break;
                    }
                    attrNode = attrNode->next;
                }

                // if elem is null, then there was no match. So insert the new attribute and set free flag to false
                if (attrNode == NULL) {
                    insertBack(img->otherAttributes, newAttribute);
                    freeFlag = false;
                }
            }

            // if free flag is set, then free the new attribute. Otherwise don't free it because it was added to the list of others
            if (freeFlag) {
                free(newAttribute->name);
                free(newAttribute);
            }

            return true;

        case CIRC:

            // loop elemIndex times to get the correct element in the list
            otherIterator = createIterator(img->circles);
            for (int i = 0; i <= elemIndex; i++) {
                elem = nextElement(&otherIterator);
                if (elem == NULL)
                    return false;

                currCircle = (Circle*)elem;
            }

            if (strcmp(newAttribute->name, "cx") == 0) {  // check for cx
                if (!validNumber(newAttribute->value))    // make sure the number is valid
                    return false;
                currCircle->cx = atof(newAttribute->value);
            }
            else if (strcmp(newAttribute->name, "cy") == 0) {  // check for cy
                if (!validNumber(newAttribute->value))         // make sure the number is valid
                    return false;
                currCircle->cy = atof(newAttribute->value);
            }
            else if (strcmp(newAttribute->name, "r") == 0) {  // check for r
                if (!validNumber(newAttribute->value))        // make sure the number is valid
                    return false;
                currCircle->r = atof(newAttribute->value);
            }
            else {
                Node* attrNode = currCircle->otherAttributes->head;
                while (attrNode != NULL) {
                    currAttribute = (Attribute*)attrNode->data;
                    if (strcmp(currAttribute->name, newAttribute->name) == 0) {
                        currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(newAttribute->value) + 1);
                        strcpy(currAttribute->value, newAttribute->value);
                        attrNode->data = currAttribute;
                        break;
                    }
                    attrNode = attrNode->next;
                }

                // if elem is null, that means we didn't find a match, in the other attributes list, so add new attribute and set freeflag to false
                if (attrNode == NULL) {
                    insertBack(currCircle->otherAttributes, newAttribute);
                    freeFlag = false;
                }
            }

            // if the free flag is set, then free the new attribute, otherwise it's now in the list so don't free it
            if (freeFlag) {
                free(newAttribute->name);
                free(newAttribute);
            }

            return true;

        case RECT:
            // loop elemIndex times to get the correct element in the list
            otherIterator = createIterator(img->rectangles);
            for (int i = 0; i <= elemIndex; i++) {
                elem = nextElement(&otherIterator);
                if (elem == NULL)
                    return false;

                currRect = (Rectangle*)elem;
            }

            if (strcmp(newAttribute->name, "x") == 0) {  // check for x
                if (!validNumber(newAttribute->value))   // make sure the number is valid
                    return false;
                currRect->x = atof(newAttribute->value);
            }
            else if (strcmp(newAttribute->name, "y") == 0) {  // check for y
                if (!validNumber(newAttribute->value))        // make sure the number is valid
                    return false;
                currRect->y = atof(newAttribute->value);
            }
            else if (strcmp(newAttribute->name, "width") == 0) {  // check for width
                if (!validNumber(newAttribute->value))            // make sure the number is valid
                    return false;
                currRect->width = atof(newAttribute->value);
            }
            else if (strcmp(newAttribute->name, "height") == 0) {  // check for height
                if (!validNumber(newAttribute->value))             // make sure the number is valid
                    return false;
                currRect->height = atof(newAttribute->value);
            }
            else {
                Node* attrNode = currRect->otherAttributes->head;
                while (attrNode != NULL) {
                    currAttribute = (Attribute*)attrNode->data;
                    if (strcmp(currAttribute->name, newAttribute->name) == 0) {
                        currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(newAttribute->value) + 1);
                        strcpy(currAttribute->value, newAttribute->value);
                        attrNode->data = currAttribute;
                        break;
                    }
                    attrNode = attrNode->next;
                }

                // if elem is null, that means we didn't find a match, in the other attributes list, so add new attribute and set freeflag to false
                if (attrNode == NULL) {
                    printf("new attribute '%s'\n", newAttribute->name);
                    insertBack(currRect->otherAttributes, newAttribute);
                    freeFlag = false;
                }
            }

            // if the free flag is set, then free the new attribute, otherwise it's now in the list so don't free it
            if (freeFlag) {
                free(newAttribute->name);
                free(newAttribute);
            }

            return true;

        case PATH:
            // loop elemIndex times to get the correct element in the list
            otherIterator = createIterator(img->paths);
            for (int i = 0; i <= elemIndex; i++) {
                elem = nextElement(&otherIterator);
                if (elem == NULL)
                    return false;

                currPath = (Path*)elem;
            }

            if (strcmp(newAttribute->name, "d") == 0) {  // check for data
                strcpy(currPath->data, newAttribute->value);
            }
            else {
                // loop thorugh and check all attributes in the list
                Node* attrNode = currPath->otherAttributes->head;
                while (attrNode != NULL) {
                    currAttribute = (Attribute*)attrNode->data;
                    if (strcmp(currAttribute->name, newAttribute->name) == 0) {
                        currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(newAttribute->value) + 1);
                        strcpy(currAttribute->value, newAttribute->value);
                        attrNode->data = currAttribute;
                        break;
                    }
                    attrNode = attrNode->next;
                }

                // if elem is null, that means we didn't find a match, in the other attributes list, so add new attribute and set freeflag to false
                if (attrNode == NULL) {
                    insertBack(currPath->otherAttributes, newAttribute);
                    freeFlag = false;
                }
            }

            // if the free flag is set, then free the new attribute, otherwise it's now in the list so don't free it
            if (freeFlag) {
                free(newAttribute->name);
                free(newAttribute);
            }

            return true;

        case GROUP:
            // loop elemIndex times to get the correct element in the list
            otherIterator = createIterator(img->groups);
            for (int i = 0; i <= elemIndex; i++) {
                elem = nextElement(&otherIterator);
                if (elem == NULL)
                    return false;

                currGroup = (Group*)elem;
            }

            // loop thorugh and check all attributes in the list
            Node* attrNode = currGroup->otherAttributes->head;
            while (attrNode != NULL) {
                currAttribute = (Attribute*)attrNode->data;
                if (strcmp(currAttribute->name, newAttribute->name) == 0) {
                    currAttribute = realloc(currAttribute, sizeof(Attribute) + strlen(newAttribute->value) + 1);
                    strcpy(currAttribute->value, newAttribute->value);
                    attrNode->data = currAttribute;
                    break;
                }
                attrNode = attrNode->next;
            }
            // if elem is null, that means we didn't find a match, in the other attributes list, so add new attribute and set freeflag to false
            if (attrNode == NULL) {
                insertBack(currGroup->otherAttributes, newAttribute);
                freeFlag = false;
            }

            // if the free flag is set, then free the new attribute, otherwise it's now in the list so don't free it
            if (freeFlag) {
                free(newAttribute->name);
                free(newAttribute);
            }

            return true;

        default:
            printf("Something very very wrong has occurred, or %d isn't a type\n", elemType);
            break;
    }

    return false;
}

void addComponent(SVG* img, elementType type, void* newElement) {
    // if img or new element are null, return and don't change anything
    if (img == NULL || newElement == NULL) {
        return;
    }

    // depending on type, insert the new element to the back of the correct list
    switch (type) {
        case CIRC:
            insertBack(img->circles, newElement);
            break;

        case RECT:
            insertBack(img->rectangles, newElement);
            break;

        case PATH:
            insertBack(img->paths, newElement);
            break;

        default:
            printf("Something went very wrong, or invalid element type");
    }
}

char* attrToJSON(const Attribute* a) {
    char* newJSON;

    // if a is null return {}
    if (a == NULL) {
        return "{}";
    }

    // len of name and value, + 25 for extra chars
    int len = strlen(a->name) + strlen(a->value) + 25;
    newJSON = (char*)malloc(sizeof(char) * len);

    // sprintf to newJSON
    sprintf(newJSON, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);

    return newJSON;
}

char* circleToJSON(const Circle* c) {
    char* newJSON;

    // if c is null return {}
    if (c == NULL) {
        return "{}";
    }

    // 16 per float, 8 per int, len of units, 45 extra
    int len = (16 * 3) + 8 + strlen(c->units) + 45;
    newJSON = (char*)malloc(sizeof(char) * len);

    // sprintf to newJSON
    sprintf(newJSON, "{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", c->cx, c->cy, c->r, getLength(c->otherAttributes), c->units);

    return newJSON;
}

char* rectToJSON(const Rectangle* r) {
    char* newJSON;

    // if r is null return {}
    if (r == NULL) {
        return "{}";
    }

    // 16 per float, 8 per int, len of units, 45 extra
    int len = (16 * 4) + 8 + strlen(r->units) + 60;
    newJSON = (char*)malloc(sizeof(char) * len);

    // sprintf to newJSON
    sprintf(newJSON, "{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}", r->x, r->y, r->width, r->height, getLength(r->otherAttributes), r->units);

    return newJSON;
}

char* pathToJSON(const Path* p) {
    char* newJSON;
    int dataLen;

    // if p is null return {}
    if (p == NULL) {
        return "{}";
    }

    dataLen = strlen(p->data);

    // datalen + 8 + 25 if datalen <= 64, otherwise just cap at 64
    int len = dataLen > 64 ? 64 + 8 + 25 : dataLen + 8 + 25;
    newJSON = (char*)malloc(sizeof(char) * len);

    // sprintf to newJSON
    sprintf(newJSON, "{\"d\":\"%.64s\",\"numAttr\":%d}", p->data, getLength(p->otherAttributes));

    return newJSON;
}

char* groupToJSON(const Group* g) {
    char* newJSON;

    // if g is null return {}
    if (g == NULL) {
        return "{}";
    }

    // 8 per int + 25 extra
    int len = 2 * 8 + 25;
    newJSON = (char*)malloc(sizeof(char) * len);

    // sprintf to newJSON
    sprintf(newJSON, "{\"children\":%d,\"numAttr\":%d}", (getLength(g->rectangles) + getLength(g->circles) + getLength(g->paths) + getLength(g->groups)), getLength(g->otherAttributes));

    return newJSON;
}

char* attrListToJSON(const List* list) {
    char* newJSON;
    char* currString;
    ListIterator attrIterator;
    Attribute* currAttr;
    void* elem;
    int len;

    // if the list is null, return []
    if (list == NULL)
        return "[]";

    len = 10;                              // set len to 10
    newJSON = malloc(sizeof(char) * len);  // malloc newJSON
    sprintf(newJSON, "[");                 // add the first [

    // iterate throught the list and attach each attributes json to the new string
    attrIterator = createIterator((List*)list);
    for (elem = nextElement(&attrIterator); elem != NULL; elem = nextElement(&attrIterator)) {
        currAttr = (Attribute*)elem;        // get curr attribute
        currString = attrToJSON(currAttr);  // get curr string

        // if the len is 10, don't add a command to the string
        if (len != 10)
            strcat(newJSON, ",");

        len += strlen(currString) + 1;                   // add the length of the current string to len
        newJSON = realloc(newJSON, sizeof(char) * len);  // realloc newJSON to have space for the current string

        strcat(newJSON, currString);  // concatenate the current string
        free(currString);             // free the current string
    }

    strcat(newJSON, "]");  // add the closing ]

    return newJSON;
}

char* circListToJSON(const List* list) {
    char* newJSON;
    char* currString;
    ListIterator circleIterator;
    Circle* currCircle;
    void* elem;
    int len;

    // if the list is null, return []
    if (list == NULL)
        return "[]";

    len = 3;                               // set len to 3 for the 2 [] and null
    newJSON = malloc(sizeof(char) * len);  // malloc newJSON
    sprintf(newJSON, "[");                 // add the first [

    // iterate throught the list and attach each circle json to the new string
    circleIterator = createIterator((List*)list);
    for (elem = nextElement(&circleIterator); elem != NULL; elem = nextElement(&circleIterator)) {
        currCircle = (Circle*)elem;             // get curr circle
        currString = circleToJSON(currCircle);  // get curr string

        // if the len is 3, do not add a command to the string
        if (len != 3)
            strcat(newJSON, ",");

        len += strlen(currString) + 1;                   // add the length of the current string to len
        newJSON = realloc(newJSON, sizeof(char) * len);  // realloc newJSON to have space for the current string

        strcat(newJSON, currString);  // concatenate the current string
        free(currString);             // free the current string
    }

    strcat(newJSON, "]");  // add the closing ]

    return newJSON;
}

char* rectListToJSON(const List* list) {
    char* newJSON;
    char* currString;
    ListIterator rectIterator;
    Rectangle* currRect;
    void* elem;
    int len;

    // if the list is null, return []
    if (list == NULL)
        return "[]";

    len = 3;                               // set len to 3 for the 2 [] and null
    newJSON = malloc(sizeof(char) * len);  // malloc newJSON
    sprintf(newJSON, "[");                 // add the first [

    // iterate throught the list and attach each rect json to the new string
    rectIterator = createIterator((List*)list);
    for (elem = nextElement(&rectIterator); elem != NULL; elem = nextElement(&rectIterator)) {
        currRect = (Rectangle*)elem;        // get curr rect
        currString = rectToJSON(currRect);  // get curr string

        // if the len is 3, do not add a command to the string
        if (len != 3)
            strcat(newJSON, ",");

        len += strlen(currString) + 1;                   // add the length of the current string to len
        newJSON = realloc(newJSON, sizeof(char) * len);  // realloc newJSON to have space for the current string

        strcat(newJSON, currString);  // concatenate the current string
        free(currString);             // free the current string
    }

    strcat(newJSON, "]");  // add the closing ]

    return newJSON;
}

char* pathListToJSON(const List* list) {
    char* newJSON;
    char* currString;
    ListIterator pathIterator;
    Path* currPath;
    void* elem;
    int len;

    // if the list is null, return []
    if (list == NULL)
        return "[]";

    len = 3;                               // set len to 3 for the 2 [] and null
    newJSON = malloc(sizeof(char) * len);  // malloc newJSON
    sprintf(newJSON, "[");                 // add the first [

    // iterate throught the list and attach each Path json to the new string
    pathIterator = createIterator((List*)list);
    for (elem = nextElement(&pathIterator); elem != NULL; elem = nextElement(&pathIterator)) {
        currPath = (Path*)elem;             // get curr Path
        currString = pathToJSON(currPath);  // get curr string

        // if the len is 3, do not add a command to the string
        if (len != 3)
            strcat(newJSON, ",");

        len += strlen(currString) + 1;                   // add the length of the current string to len
        newJSON = realloc(newJSON, sizeof(char) * len);  // realloc newJSON to have space for the current string

        strcat(newJSON, currString);  // concatenate the current string
        free(currString);             // free the current string
    }

    strcat(newJSON, "]");  // add the closing ]

    return newJSON;
}

char* groupListToJSON(const List* list) {
    char* newJSON;
    char* currString;
    ListIterator groupIterator;
    Group* currGroup;
    void* elem;
    int len;

    // if the list is null, return []
    if (list == NULL)
        return "[]";

    len = 3;                               // set len to 3 for the 2 [] and null
    newJSON = malloc(sizeof(char) * len);  // malloc newJSON
    sprintf(newJSON, "[");                 // add the first [

    // iterate throught the list and attach each Group json to the new string
    groupIterator = createIterator((List*)list);
    for (elem = nextElement(&groupIterator); elem != NULL; elem = nextElement(&groupIterator)) {
        currGroup = (Group*)elem;             // get curr Group
        currString = groupToJSON(currGroup);  // get curr string

        // if the len is 3, do not add a command to the string
        if (len != 3)
            strcat(newJSON, ",");

        len += strlen(currString) + 1;                   // add the length of the current string to len
        newJSON = realloc(newJSON, sizeof(char) * len);  // realloc newJSON to have space for the current string

        strcat(newJSON, currString);  // concatenate the current string
        free(currString);             // free the current string
    }

    strcat(newJSON, "]");  // add the closing ]

    return newJSON;
}

char* SVGtoJSON(const SVG* img) {
    char* newJSON;
    int numRects = 0;
    int numCircles = 0;
    int numPaths = 0;
    int numGroups = 0;
    List* currList;

    // if img is null return {}
    if (img == NULL) {
        return "{}";
    }

    // Get the total rects in the svg
    currList = getRects(img);
    numRects = getLength(currList);
    freeList(currList);

    // Get the total Circles in the svg
    currList = getCircles(img);
    numCircles = getLength(currList);
    freeList(currList);

    // Get the total Paths in the svg
    currList = getPaths(img);
    numPaths = getLength(currList);
    freeList(currList);

    // Get the total Groups in the svg
    currList = getGroups(img);
    numGroups = getLength(currList);
    freeList(currList);

    // 8 per int + 40 extra
    int len = 4 * 8 + 40;
    newJSON = (char*)malloc(sizeof(char) * len);

    // sprintf to newJSON
    sprintf(newJSON, "{\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d}", numRects, numCircles, numPaths, numGroups);

    return newJSON;
}

//~~~~~~~~ Helper Functions ~~~~~~~~//
// Attributes
void deleteAttribute(void* data) {
    Attribute* tmpData;

    // if the data is null, do nothing
    if (data == NULL) {
        return;
    }

    // get the data
    tmpData = (Attribute*)data;

    // free the name and the data.
    free(tmpData->name);
    // free(tmpData->value);
    free(tmpData);
}

char* attributeToString(void* data) {
    char* tmpStr;
    Attribute* tmpData;
    int len;

    // if the data is null, do nothing
    if (data == NULL) {
        return NULL;
    }

    // get the data
    tmpData = (Attribute*)data;

    // make the size the len of the 2 strings, plus 20 for other chars
    len = strlen(tmpData->name) + strlen(tmpData->value) + 20;
    tmpStr = (char*)malloc(len * sizeof(char));

    // print the data to the tmp string
    sprintf(tmpStr, "Name: %s\n Data: %s\n", tmpData->name, tmpData->value);

    return tmpStr;
}

int compareAttributes(const void* first, const void* second) {
    return 0;
}

// Groups
void deleteGroup(void* data) {
    if (data == NULL)  // if data is null, there is nothing to free
        return;

    Group* tmpData;
    tmpData = (Group*)data;

    // free the 5 lists
    freeList(tmpData->rectangles);
    freeList(tmpData->circles);
    freeList(tmpData->paths);
    freeList(tmpData->groups);
    freeList(tmpData->otherAttributes);

    // free the group itself
    free(tmpData);
}

char* groupToString(void* data) {
    // if the data is null there is nothing to free
    if (data == NULL)
        return NULL;

    char* tmpStr;
    Group* tmpData;
    char* tmpRectStr;
    char* tmpCircleStr;
    char* tmpPathStr;
    char* tmpGroupStr;
    char* tmpAttrStr;
    int len;

    // get the data
    tmpData = (Group*)data;

    // to string the lists
    tmpRectStr = toString(tmpData->rectangles);
    tmpCircleStr = toString(tmpData->circles);
    tmpPathStr = toString(tmpData->paths);
    tmpGroupStr = toString(tmpData->groups);
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size the len of each string, plus 100 for extra chars.
    len = strlen(tmpRectStr) + strlen(tmpCircleStr) + strlen(tmpPathStr) + strlen(tmpGroupStr) + strlen(tmpAttrStr) + 100;
    tmpStr = (char*)malloc(len * sizeof(char));

    // print the data to the tmp string
    sprintf(tmpStr, "\nRectangles:\n%s\nCircles:\n%s\nPaths:\n%s\nGroups:\n%s\nOther Attributes:\n%s\n", tmpRectStr, tmpCircleStr, tmpPathStr, tmpGroupStr, tmpAttrStr);

    // free the temp strings
    free(tmpRectStr);
    free(tmpCircleStr);
    free(tmpPathStr);
    free(tmpGroupStr);
    free(tmpAttrStr);

    return tmpStr;
}

int compareGroups(const void* first, const void* second) {
    // do nothing right now
    return 0;
}

// Rectangle
void deleteRectangle(void* data) {
    if (data == NULL)
        return;

    Rectangle* tmpData;

    tmpData = (Rectangle*)data;

    freeList(tmpData->otherAttributes);  // free the only list
    free(tmpData);                       // free the rect itself
}

char* rectangleToString(void* data) {
    // if the data is null there is nothing to free
    if (data == NULL)
        return NULL;

    char* tmpStr;
    char* tmpAttrStr;
    Rectangle* tmpData;
    int len;

    // get the data and list string
    tmpData = (Rectangle*)data;
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size 30 per float, plus the len of units and attributes, plus 30 for extra chars
    len = (4 * 30) + strlen(tmpData->units) + strlen(tmpAttrStr) + 30;
    tmpStr = (char*)malloc(len * sizeof(char));

    // print the data to the tmp string
    sprintf(tmpStr, "x: %lf\ny: %lf\nwidth: %lf\nheight: %lf\nUnits: %s\nOther Attributes:\n%s", tmpData->x, tmpData->y, tmpData->width, tmpData->height, tmpData->units, tmpAttrStr);

    // free the temp string
    free(tmpAttrStr);

    return tmpStr;
}

int compareRectangles(const void* first, const void* second) {
    return 0;
}

// Circles
void deleteCircle(void* data) {
    // if the data is null there is nothing to free
    if (data == NULL)
        return;

    Circle* tmpData;

    // get the data
    tmpData = (Circle*)data;

    freeList(tmpData->otherAttributes);  // free the only list
    free(tmpData);                       // free the circle itself
}

char* circleToString(void* data) {
    // if the data is null there is nothing to free
    if (data == NULL)
        return NULL;

    char* tmpStr;
    char* tmpAttrStr;
    Circle* tmpData;
    int len;

    // get the data and list string
    tmpData = (Circle*)data;
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size 30 per float, plus the len of units and attributes, plus 30 for other chars
    len = (3 * 30) + strlen(tmpData->units) + strlen(tmpAttrStr) + 30;
    tmpStr = (char*)malloc(len * sizeof(char));  // malloc the space

    // print the data to the tmp string
    sprintf(tmpStr, "cx: %lf\ncy: %lf\nr: %lf\nUnits: %s\nOther Attributes:\n%s", tmpData->cx, tmpData->cy, tmpData->r, tmpData->units, tmpAttrStr);

    // free the temp string
    free(tmpAttrStr);

    return tmpStr;
}

int compareCircles(const void* first, const void* second) {
    // do nothing right now
    return 0;
}

// Paths
void deletePath(void* data) {
    // if the data is null there is nothing to free
    if (data == NULL)
        return;

    Path* tmpData;

    // get the data
    tmpData = (Path*)data;

    freeList(tmpData->otherAttributes);  // free the only list
    free(tmpData);                       // free the path itself
}

char* pathToString(void* data) {
    // if the data is null there is nothing to free
    if (data == NULL)
        return NULL;

    char* tmpStr;
    char* tmpAttrStr;
    Path* tmpData;
    int len;

    // get the data and list string
    tmpData = (Path*)data;
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size the len of the 2 strings, plus 40 for other chars
    len = strlen(tmpData->data) + strlen(tmpAttrStr) + 40;
    tmpStr = (char*)malloc(len * sizeof(char));  // malloc the tmp string

    // print the data to the tmp string
    sprintf(tmpStr, "Other Attributes:\n%sData: %s\n", tmpAttrStr, tmpData->data);

    // free the tmp attr string
    free(tmpAttrStr);

    return tmpStr;
}

int comparePaths(const void* first, const void* second) {
    // do nothing right now
    return 0;
}