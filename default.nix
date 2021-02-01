{
  pkgs ? import <nixpkgs> {}
}: with pkgs; {

  site = stdenv.mkDerivation {
    name = "site";
    index = builtins.toFile "index.html" (builtins.toXML {
      a = 5;
    });
    buildCommand = "mkdir $out; cp $index $out";
  };

}
