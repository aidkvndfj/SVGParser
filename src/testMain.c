#include "LinkedListAPI.h"
#include "SVGHelpers.h"
#include "SVGParser.h"

int main(void) {
    SVG* SVG1;
    // SVG* SVG2;
    SVG1 = createValidSVG("rects.svg", "svg.xsd");
    printf("%d\n", validateSVG(SVG1, "svg.xsd"));

    Attribute* newAttribute = malloc(sizeof(Attribute) + 22);
    newAttribute->name = malloc(sizeof(char) * 5);
    strcpy(newAttribute->name, "fill\0");
    strcpy(newAttribute->value, "red\0");

    setAttribute(SVG1, RECT, 0, newAttribute);
    printf("Update Success ");
    printf("%d\n", validateSVG(SVG1, "svg.xsd"));

    writeSVG(SVG1, "test.svg");

    // SVG2 = createValidSVG("test.svg", "svg.xsd");

    // printf("%d\n", validateSVG(SVG2, "svg.xsd"));

    /*
    Attribute* newAttribute = malloc(sizeof(Attribute) + 22);
    newAttribute->name = malloc(sizeof(char) * 2);
    strcpy(newAttribute->name, "x\0");
    strcpy(newAttribute->value, "12345678900987654321\0");

    printf("%d\n", validateSVG(SVG2, "svg.xsd"));

    char* currString = attrToJSON(newAttribute);
    printf("%s | %s\n", newAttribute->name, newAttribute->value);
    printf("%s\n", currString);
    free(currString);

    printf("First String\n");
    currString = groupListToJSON(SVG2->groups);
    printf("%s\n", currString);
    free(currString);

    setAttribute(SVG2, RECT, 1, newAttribute);

    printf("2nd String\n");
    currString = groupListToJSON(SVG2->groups);
    printf("%s\n", currString);
    free(currString);

    writeSVG(SVG2, "test.svg");
*/

    deleteSVG(SVG1);
    // deleteSVG(SVG2);

    return 1;
}