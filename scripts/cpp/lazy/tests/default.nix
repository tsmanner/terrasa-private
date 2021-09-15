{
  pkgs ? import <nixpkgs> {},
  compiler ? pkgs.clang_11
}:
pkgs.stdenv.mkDerivation rec {
  name = "lazy-test";
  buildInputs = [
    pkgs.rapidcheck
    pkgs.fmt
  ];
  src = ./.;
  flags = [
    "-g"
    "-Wall"
    "-Wextra"
    "-Werror"
    "-std=c++20"
    "-I${../include}"
    "-I${./.}"
  ];
  sources = [
    "${./main.cpp}"
    "${./test_Random.cpp}"
    "${./test_UnaryOperator.cpp}"
    "${./test_BinaryOperator.cpp}"
  ];
  libraries = [
    "-lrapidcheck"
    "-lfmt"
  ];
  command = ''${compiler}/bin/c++ ${toString flags} ${toString sources} -o $out ${toString libraries}'';
  buildCommand = ''
    echo ${command}
    ${command}
  '';
}
