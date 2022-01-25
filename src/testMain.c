#include "SVGHelpers.h"
#include "SVGParser.h"

int main(void) {
    SVG* SVG1;

    SVG1 = createSVG("quad01.svg");
    deleteSVG(SVG1);

    return 1;
}
