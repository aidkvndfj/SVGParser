#include "SVGParser.h"

#include "SVGHelpers.h"

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

    // set the namepsace, title, and desc. to empty string
    strcpy(newSVG->namespace, " ");
    strcpy(newSVG->title, " ");
    strcpy(newSVG->description, " ");

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
    sprintf(tmpStr, "Namespace: %s\nTitle: %s\nDescrpition: %s\n\n", img->namespace, img->title, img->description);  // add namespace, title, and desc
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

    List* totalRects = initializeList(rectangleToString, deleteRectangle, compareRectangles);
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

    List* totalCircles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
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

// Function that returns a list of all paths in the struct.
List* getPaths(const SVG* img) {
    if (img == NULL)
        return NULL;

    void* elem;
    Path* currPath;
    Group* currGroup;

    List* totalPaths = initializeList(rectangleToString, deleteRectangle, compareRectangles);
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

// Function that returns a list of all groups in the struct.
List* getGroups(const SVG* img) {
    if (img == NULL)
        return NULL;

    void* elem;
    Group* currGroup;

    List* totalGroups = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    ListIterator groupIter = createIterator(img->groups);

    // loop through all the groups and add them to the total group list, then add any groups inside the groups to the groups list
    while ((elem = nextElement(&groupIter)) != NULL) {
        currGroup = (Group*)elem;

        insertBack(totalGroups, currGroup);
        addGroupGroups(totalGroups, currGroup);
    }

    return totalGroups;
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
    freeListKeepData(tmpRects);
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
    freeListKeepData(tmpCircle);
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
    freeListKeepData(tmpPaths);
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
        currLen = 0;                                  // set curr len to 0
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
    freeListKeepData(tmpGroups);
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
    freeListKeepData(tmpRect);
    freeListKeepData(tmpCircle);
    freeListKeepData(tmpPath);
    freeListKeepData(tmpGroup);

    return currLen;
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

    // make the size 30 per flost, plus the len of units and attributes, plus 30 for other chars
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