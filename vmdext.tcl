
package provide vmdext 0.1

load [file join [file dirname [file normalize [info script]]] mindist.so]

proc atomselect_by_resid {molid txt} {
  global global_sels 

  set sel [atomselect $molid $txt] 
  set resids [lsort -unique -integer [$sel get resid]]
  $sel delete

  set sels {}
  foreach resid $resids {
    set sel [uplevel 1 "atomselect $molid \"$txt and resid $resid\""]
    lappend sels $sel
  }
  return $sels
}

proc fitrmsd {sel1 sel2} {
  $sel1 move [measure fit $sel1 $sel]
  return [measure rmsd $sel1 $sel2] 
}

proc mindist {sel1 sel2} {
  set coords1 [$sel1 get {x y z}]
  set coords2 [$sel2 get {x y z}]

  return [c_mindist $coords1 $coords2]
}



