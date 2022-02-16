#include "LinkedListAPI.h"
#include "SVGHelpers.h"
#include "SVGParser.h"

int main(void) {
    SVG* SVG1;
    SVG1 = createSVG("beer.svg");

    // char* svgStr;
    // svgStr = SVGToString(SVG1);
    // printf("%s", svgStr);
    // free(svgStr);

    // printf("%d\n", getLength(getGroups(SVG1)));
    printf("%d\n", numAttr(SVG1));
    // printf("%d\n", numGroupsWithLen(SVG1, 1.0));

    deleteSVG(SVG1);

    return 1;
}