
package require vmdext

source vmdext.tcl

mol new 3VE6.pdb waitfor all
set resids1 [atomselect_by_resid top "chain A"]
set resids2 [atomselect_by_resid top "chain B"]
if {([info exists resids1] == 0) || ([info exists resids2] == 0)} {
  puts "Error: failed to get selections."
  exit
}

# mindist
foreach resid1 $resids1 {
  foreach resid2 $resids2 {
    set r1 [mindist $resid1 $resid2]
    set r2 50000.
    foreach xyz1 [$resid1 get {x y z}] {
      foreach xyz2 [$resid2 get {x y z}] {
        set r [vecdist $xyz1 $xyz2]
        if {$r < $r2} {
          set r2 $r
        }
      }
    }
    if {[expr abs($r1 - $r2)] > 1e-7} {
      puts "Error: mindist ($r1 vs $r2)"
      exit
    } 
  }
}

puts "All tests passed!"

exit
