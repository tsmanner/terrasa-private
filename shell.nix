{ pkgs ? import <nixpkgs> {} }:
let

  modules = (ps: with ps; [
    base
    random
  ]);

in pkgs.mkShell {
  buildInputs = [
    (pkgs.ghc.withPackages modules)
    pkgs.gdb
    pkgs.cabal-install
  ];
}
