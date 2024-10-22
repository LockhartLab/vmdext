
#include <tcl/tcl.h>
#include <math.h>
#include <stdlib.h>  

int c_mindist(ClientData clientdata, Tcl_Interp *interp, int argc, const char *argv[]) {
    if (argc != 3) {
        Tcl_SetResult(interp, "Usage: c_mindist coords1 coords2", TCL_STATIC);
        return TCL_ERROR;
    }

    // Parse the coordinates from Tcl arguments
    Tcl_Obj *coords1_list = Tcl_NewStringObj(argv[1], -1);
    Tcl_Obj *coords2_list = Tcl_NewStringObj(argv[2], -1);

    int n1, n2;
    double *coords1, *coords2;
    
    // Convert Tcl lists to arrays of doubles
    Tcl_ListObjLength(interp, coords1_list, &n1);
    Tcl_ListObjLength(interp, coords2_list, &n2);
    coords1 = malloc(3 * n1 * sizeof(double));
    coords2 = malloc(3 * n2 * sizeof(double));

    for (int i = 0; i < n1; i++) {
        Tcl_Obj *sublist;
        if (Tcl_ListObjIndex(interp, coords1_list, i, &sublist) != TCL_OK) {
            Tcl_AddErrorInfo(interp, "Failed to get sublist from coords1_list");
            return TCL_ERROR;
        }
        for (int j = 0; j < 3; j++) {
            Tcl_Obj *elem;
            if (Tcl_ListObjIndex(interp, sublist, j, &elem) != TCL_OK) {
                Tcl_AddErrorInfo(interp, "Failed to get element from sublist");
                return TCL_ERROR;
            }
            if (Tcl_GetDoubleFromObj(interp, elem, &coords1[3 * i + j]) != TCL_OK) {
                Tcl_AddErrorInfo(interp, "Failed to get double from sublist element");
                return TCL_ERROR;
            }
        }
    }
    
    for (int i = 0; i < n2; i++) {
        Tcl_Obj *sublist;
        if (Tcl_ListObjIndex(interp, coords2_list, i, &sublist) != TCL_OK) {
            Tcl_AddErrorInfo(interp, "Failed to get sublist from coords2_list");
            return TCL_ERROR;
        }
        for (int j = 0; j < 3; j++) {
            Tcl_Obj *elem;
            if (Tcl_ListObjIndex(interp, sublist, j, &elem) != TCL_OK) {
                Tcl_AddErrorInfo(interp, "Failed to get element from sublist");
                return TCL_ERROR;
            }
            if (Tcl_GetDoubleFromObj(interp, elem, &coords2[3 * i + j]) != TCL_OK) {
                Tcl_AddErrorInfo(interp, "Failed to get double from sublist element");
                return TCL_ERROR;
            }
        }
    }

    // Do calculation
    double min_dist = 1e12; // Start with a large number
    for (int i = 0; i < n1; i++) {
        double x1 = coords1[3*i];
        double y1 = coords1[3*i + 1];
        double z1 = coords1[3*i + 2];

        for (int j = 0; j < n2; j++) {
            double x2 = coords2[3*j];
            double y2 = coords2[3*j + 1];
            double z2 = coords2[3*j + 2];

            // Calculate Euclidean distance between points i and j
            double dist = sqrt((x2 - x1) * (x2 - x1) +
                               (y2 - y1) * (y2 - y1) +
                               (z2 - z1) * (z2 - z1));

            if (dist < min_dist) {
                min_dist = dist;
            }
        }
    }

    // Free memory
    free(coords1);
    free(coords2);

    // Return the result to Tcl
    Tcl_SetObjResult(interp, Tcl_NewDoubleObj(min_dist));
    return TCL_OK;
}

// Create command
int Mindist_Init(Tcl_Interp *interp) {
    Tcl_CreateCommand(interp, "c_mindist", c_mindist, NULL, NULL);
    return TCL_OK;
}
