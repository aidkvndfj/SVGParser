#ifndef SVG_HELPERS_H
#define SVG_HELPERS_H

#include <libxml/encoding.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/xmlwriter.h>
#include <math.h>

#include "SVGParser.h"

void parseSVG(xmlNode* rootNode, SVG* currSVG, Group* currGroup);
float removeUnits(char* string, char* unitsStr);
Group* createNewGroup();

#endif
