#include <stdio.h>

#include <grid.h>
#include <diffreader.h>

int main(int argc, char ** argv)
{
				Grid g1,g2;
				DpReader dr;
				DpGridFileInfo dgfi;

				if (argc < 2) {
					printf("Please anter filename to read.\nExample:\n");
					printf("./grt in_file.grid out_file.grid\n");
					exit(-1);
				}
				
				dgfi.ReadFromFile(argv[1]);
				//for (int i = 1; i <= dgfi.gridCount(); i++)
				dgfi.debug();
				//printf("Ala.\n");

				dr.ReadFromFile(&g1,argv[1],dgfi.getInfo(1).position);
//				g1.debug();
				//printf("Ala.\n");
//				g1.boundaryNodes( 200 );
				dr.SaveToFile(&g1,argv[2]);
/*				dr.ReadFromFile(&g2,argv[1],dgfi.getInfo(1).position);
				g2.debug();
*/
				//printf("Ala.\n");
				

/*				int i = 0;
				while (i<g1._elems.length()) {
					g1._elems[i].debug();
					i++;
				}
*/
				return(0);
}
