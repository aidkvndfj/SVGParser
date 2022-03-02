#include "LinkedListAPI.h"
#include "SVGHelpers.h"
#include "SVGParser.h"

int main(void) {
    SVG* SVG1;
    SVG* SVG2;
    SVG1 = createValidSVG("quad01.svg", "svg.xsd");
    printf("%d\n", validateSVG(SVG1, "svg.xsd"));

    if (SVG1 != NULL) {
        char* svgStr;
        svgStr = SVGToString(SVG1);
        printf("%s", svgStr);
        free(svgStr);

        writeSVG(SVG1, "test.svg");
    }

    SVG2 = createValidSVG("test.svg", "svg.xsd");

    printf("%d\n", validateSVG(SVG2, "svg.xsd"));

    Attribute* newAttribute = malloc(sizeof(Attribute) + 22);
    newAttribute->name = malloc(sizeof(char) * 2);
    strcpy(newAttribute->name, "x\0");
    strcpy(newAttribute->value, "12345678900987654321\0");

    if (!setAttribute(SVG2, RECT, 1, newAttribute)) {
        free(newAttribute->name);
        free(newAttribute);
    }

    printf("%d\n", validateSVG(SVG2, "svg.xsd"));
    writeSVG(SVG2, "test.svg");

    deleteSVG(SVG1);
    deleteSVG(SVG2);

    return 1;
}