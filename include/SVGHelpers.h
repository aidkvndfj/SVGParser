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

/**
 * @brief Parse the SVG file for all elements
 *
 * @param rootNode The starting node of the tree
 * @param currSVG The current SVG to modify - can be NULL
 * @param currGroup The current group to modify - can be NULL
 */
void parseSVG(xmlNode* rootNode, SVG* currSVG, Group* currGroup);

/**
 * @brief Remove the units off of attribtues, and save them to unitsStr
 *
 * @param inputString The string to remove units off of
 * @param unitsStr The string to assign the units to
 * @return A float that is the number extracted from inputString
 */
float removeUnits(char* inputString, char* unitsStr);

/**
 * @brief Create a New Group object
 *
 * @return A new and proporly initalized group
 */
Group* createNewGroup();

/**
 * @brief Recursively go through the group, and add all rects to rectList
 *
 * @param rectList The list of rects to add to
 * @param group The group to look at
 */
void addGroupRects(List* rectList, Group* group);

/**
 * @brief Recursively go through the group, and add all rects to circleList
 *
 * @param circleList The list of circles to add to
 * @param group The group to look at
 */
void addGroupCircles(List* circleList, Group* group);

/**
 * @brief Recursively go through the group, and add all rects to pathList
 *
 * @param pathList The list of paths to add to
 * @param group The group to look at
 */
void addGroupPaths(List* pathList, Group* group);

/**
 * @brief Recursively go through the group, and add all rects to groupList
 *
 * @param groupList The list of groups to add to
 * @param group The group to look at
 */
void addGroupGroups(List* groupList, Group* group);

/**
 * @brief Count the total number of other attributes in a list of rects
 *
 * @param rectList The list of rects to count
 * @return The number of attributes in rectList
 */
int numAttrInRectList(List* rectList);

/**
 * @brief Count the total number of other attributes in a list of circles
 *
 * @param circleList The list of circles to count
 * @return The number of attributes in circleList
 */
int numAttrInCircleList(List* circleList);

/**
 * @brief Count the total number of other attributes in a list of paths
 *
 * @param pathList The list of paths to count
 * @return The number of attributes in pathList
 */
int numAttrInPathList(List* pathList);

/**
 * @brief Count the total number of other attributes in a list of groups
 *
 * @param groupList The list of groups to count
 * @return The number of attributes in groupList
 */
int numAttrInGroupList(List* groupList);

/**
 * @brief Literally does nothing
 */
void doNothing(void* data);

/**
 * @brief Creates an xml tree based on an SVG
 *
 * @param currSVG The SVG to base the new tree on
 * @param bGroup Used to recursively call this function, set to NULL
 * @param rootNode Used to recursively call this function, set to NULL
 * @return A xmlNodePtr that is the root node of the new tree
 */
xmlNodePtr createTree(const SVG* currSVG, Group* bGroup, xmlNodePtr rootNode);

/**
 * @brief validated a doc with a given schema file
 *
 * @param doc The xmlDocPtr to validate
 * @param schemaFile The name of the schemaFile to check against
 * @return True if valide, false otherwise or if something went wrong
 */
bool validateDoc(xmlDocPtr doc, const char* schemaFile);

/**
 * @brief Checks a string to make sure it can safely be cast to a float
 *
 * @param string The string to check
 * @return True if it can be cast to a float, false otherwise
 */
bool validNumber(char* string);

#endif
