{
  pkgs ? import <nixpkgs> {},
  compiler ? pkgs.clang_11
}: with pkgs;
stdenv.mkDerivation {
  name = "terrasa";
  src = ./.;
  buildInputs = [ fmt ];
  buildCommand = ''
    ${compiler}/bin/c++ -o $out -g -std=c++20 -fPIC -ldl -Wall -Wextra -Werror -I$src/include $src/main.cpp -lfmt
  '';
}
