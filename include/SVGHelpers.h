#ifndef SVG_HELPERS_H
#define SVG_HELPERS_H

#include <libxml/encoding.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xmlwriter.h>
#include <math.h>
#include <stdlib.h>

#include "SVGParser.h"

#define PI 3.1415926535897932384626433832795

// parse the SVG file for all elemtns
void parseSVG(xmlNode* rootNode, SVG* currSVG, Group* currGroup);

// remove the units off of attribtues
float removeUnits(char* inputString, char* unitsStr);

// initalize a new group
Group* createNewGroup();

// Recursively go through the group, and add all rects to rectList
void addGroupRects(List* rectList, Group* group);

// Recursively go through the group, and add all circles to circleList
void addGroupCircles(List* circleList, Group* group);

// Recursively go through the group, and add all paths to pathList
void addGroupPaths(List* PathList, Group* group);

// Recursively go through the group, and add all groups to groupList
void addGroupGroups(List* GroupList, Group* group);

// count the total number of other attributes in a list of rects
int numAttrInRectList(List* rectList);

// count the total number of other attributes in a list of circless
int numAttrInCircleList(List* circleList);

// count the total number of other attributes in a list of paths
int numAttrInPathList(List* pathList);

// count the total number of other attributes in a list of groups
int numAttrInGroupList(List* groupList);

void doNothing(void* data);

// creates a xmlDoc from a svg struct
xmlNodePtr createTree(const SVG* currSVG, Group* bGroup, xmlNodePtr rootNode);
// xmlDocPtr createTree(const SVG* currSVG);

xmlNodePtr createGroupNode(Group* currGroup);

bool validNumber(char* string);

#endif
