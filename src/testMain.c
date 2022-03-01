#include "LinkedListAPI.h"
#include "SVGHelpers.h"
#include "SVGParser.h"

int main(void) {
    SVG* SVG1;
    SVG1 = createValidSVG("quad01.svg", "svg.xsd");
    printf("%d\n", validateSVG(SVG1, "svg.xsd"));

    if (SVG1 != NULL) {
        char* svgStr;
        svgStr = SVGToString(SVG1);
        printf("%s", svgStr);
        free(svgStr);

        writeSVG(SVG1, "test.svg");
    }

    deleteSVG(SVG1);

    return 1;
}