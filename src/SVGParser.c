#include "SVGParser.h"

#include "SVGHelpers.h"

//~~~~~~~~ Parser Functions ~~~~~~~~//
SVG* createSVG(const char* fileName) {
    SVG* newSVG = NULL;
    xmlDoc* doc = NULL;
    xmlNode* rootElement = NULL;

    newSVG = (SVG*)malloc(sizeof(SVG));
    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        fprintf(stderr, "Could not parse file: %s\n", fileName);
        return NULL;
    }

    rootElement = xmlDocGetRootElement(doc);

    strcpy(newSVG->namespace, (char*)rootElement->ns->href);                                          // get the namespace
    newSVG->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);       // create empty rect list
    newSVG->circles = initializeList(circleToString, deleteCircle, compareCircles);                   // create empty circle list
    newSVG->paths = initializeList(pathToString, deletePath, comparePaths);                           // create empty path list
    newSVG->groups = initializeList(groupToString, deleteGroup, compareGroups);                       // create empty group list
    newSVG->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);  // create empty other list

    parseSVG(rootElement, newSVG, NULL);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return newSVG;
}

void deleteSVG(SVG* img) {
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->paths);
    freeList(img->groups);
    freeList(img->otherAttributes);
    free(img);
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
    sprintf(tmpStr, "Name: %s\n Data: %s\n\n", tmpData->name, tmpData->value);

    return tmpStr;
}

int compareAttributes(const void* first, const void* second) {
    return 0;
}

// Groups
void deleteGroup(void* data) {
    Group* tmpData;

    if (data == NULL) {
        return;
    }

    tmpData = (Group*)data;

    freeList(tmpData->rectangles);
    freeList(tmpData->circles);
    freeList(tmpData->paths);
    freeList(tmpData->groups);

    freeList(tmpData->otherAttributes);
    free(tmpData);
}

char* groupToString(void* data) {
    char* tmpStr;
    Group* tmpData;
    char* tmpRectStr;
    char* tmpCircleStr;
    char* tmpPathStr;
    char* tmpGroupStr;
    char* tmpAttrStr;
    int len;

    // if the data is null, do nothing
    if (data == NULL) {
        return NULL;
    }

    // get the data
    tmpData = (Group*)data;
    // to string the lists
    tmpRectStr = toString(tmpData->rectangles);
    tmpCircleStr = toString(tmpData->circles);
    tmpPathStr = toString(tmpData->paths);
    tmpGroupStr = toString(tmpData->groups);
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size 64 for the prints, plus 7 chars for each number.
    len = strlen(tmpRectStr) + strlen(tmpCircleStr) + strlen(tmpPathStr) + strlen(tmpGroupStr) + strlen(tmpAttrStr) + 85;
    tmpStr = (char*)malloc(len * sizeof(char));

    // print the data to the tmp string
    sprintf(tmpStr, "Rectangles:\n%sCircles:\n%sPaths:\n%sGroups:\n%sOther Attributes:\n%s", tmpRectStr, tmpCircleStr, tmpPathStr, tmpGroupStr, tmpAttrStr);

    return tmpStr;
}

int compareGroups(const void* first, const void* second) {
    return 0;
}

// Rectangle
void deleteRectangle(void* data) {
    Rectangle* tmpData;

    if (data == NULL) {
        return;
    }

    tmpData = (Rectangle*)data;
    freeList(tmpData->otherAttributes);
    free(tmpData);
}

char* rectangleToString(void* data) {
    char* tmpStr;
    char* tmpAttrStr;
    Rectangle* tmpData;
    int len;

    // if the data is null, do nothing
    if (data == NULL) {
        return NULL;
    }

    // get the data and list string
    tmpData = (Rectangle*)data;
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size the len of the 2 strings, plus 20 for other chars
    len = (4 * 30) + strlen(tmpData->units) + strlen(tmpAttrStr) + 30;
    tmpStr = (char*)malloc(len * sizeof(char));

    // print the data to the tmp string
    sprintf(tmpStr, "x: %lf\ny: %lf\nwidth: %lf\nheight: %lf\nUnits: %s\nOther Attributes:\n%s", tmpData->x, tmpData->y, tmpData->width, tmpData->height, tmpData->units, tmpAttrStr);

    return tmpStr;
}

int compareRectangles(const void* first, const void* second) {
    return 0;
}

// Circles
void deleteCircle(void* data) {
    Circle* tmpData;

    // if the data is null, do nothing
    if (data == NULL) {
        return;
    }

    // get the data
    tmpData = (Circle*)data;

    // free the name and the data.
    freeList(tmpData->otherAttributes);
    free(tmpData);
}

char* circleToString(void* data) {
    char* tmpStr;
    char* tmpAttrStr;
    Circle* tmpData;
    int len;

    // if the data is null, do nothing
    if (data == NULL) {
        return NULL;
    }

    // get the data and list string
    tmpData = (Circle*)data;
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size the len of the 2 strings, plus 20 for other chars
    len = (3 * 30) + strlen(tmpData->units) + strlen(tmpAttrStr) + 30;
    tmpStr = (char*)malloc(len * sizeof(char));

    // print the data to the tmp string
    sprintf(tmpStr, "cx: %lf\ncy: %lf\nr: %lf\nUnits: %s\nOther Attributes:\n%s", tmpData->cx, tmpData->cy, tmpData->r, tmpData->units, tmpAttrStr);

    return tmpStr;
}

int compareCircles(const void* first, const void* second) {
    return 0;
}

// Paths
void deletePath(void* data) {
    Path* tmpData;

    // if the data is null, do nothing
    if (data == NULL) {
        return;
    }

    // get the data
    tmpData = (Path*)data;

    // free the name and the data.
    freeList(tmpData->otherAttributes);
    free(tmpData);
}

char* pathToString(void* data) {
    char* tmpStr;
    char* tmpAttrStr;
    Path* tmpData;
    int len;

    // if the data is null, do nothing
    if (data == NULL) {
        return NULL;
    }

    // get the data and list string
    tmpData = (Path*)data;
    tmpAttrStr = toString(tmpData->otherAttributes);

    // make the size the len of the 2 strings, plus 20 for other chars
    len = strlen(tmpData->data) + strlen(tmpAttrStr) + 30;
    tmpStr = (char*)malloc(len * sizeof(char));

    // print the data to the tmp string
    sprintf(tmpStr, "Other Attributes:\n%sData: %s\n", tmpAttrStr, tmpData->data);

    return tmpStr;
}

int comparePaths(const void* first, const void* second) {
    return 0;
}