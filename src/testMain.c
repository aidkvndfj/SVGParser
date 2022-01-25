#include "SVGHelpers.h"
#include "SVGParser.h"

int main(void) {
    SVG* SVG1;

    SVG1 = createSVG("quad01.svg");

    char* svgStr;
    svgStr = SVGToString(SVG1);
    printf("%s", svgStr);
    free(svgStr);

    deleteSVG(SVG1);

    return 1;
}
