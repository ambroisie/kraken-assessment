{
  description = "Kraken technical assessment";

  inputs = {
    futils = {
      type = "github";
      owner = "numtide";
      repo = "flake-utils";
      ref = "master";
    };

    nixpkgs = {
      type = "github";
      owner = "NixOS";
      repo = "nixpkgs";
      ref = "nixos-unstable";
    };

    pre-commit-hooks = {
      type = "github";
      owner = "cachix";
      repo = "pre-commit-hooks.nix";
      ref = "master";
      inputs = {
        flake-utils.follows = "futils";
        nixpkgs.follows = "nixpkgs";
      };
    };
  };

  outputs = { self, futils, nixpkgs, pre-commit-hooks }:
    {
      overlay = final: prev: {
        kraken = final.stdenv.mkDerivation {
          pname = "kraken";
          version = "0.0.0";

          src = self;

          nativeBuildInputs = with final; [
            cmake
            ninja
            pkg-config
          ];

          buildInputs = with final; [
            boost
          ];

          checkInputs = with final; [
            gtest
          ];

          doCheck = true;

          meta = with final.lib; {
            description = "Order book";
            homepage = "https://gitea.belanyi.fr/ambroisie/kraken-assessment";
            license = licenses.mit;
            maintainers = [ ambroisie ];
            platforms = platforms.unix;
          };
        };
      };
    } // futils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; overlays = [ self.overlay ]; };
      in
      {
        checks = {
          inherit (self.packages.${system}) kraken;

          pre-commit = pre-commit-hooks.lib.${system}.run {
            src = self;

            hooks = {
              nixpkgs-fmt = {
                enable = true;
              };

              clang-format = {
                enable = true;
                name = "clang-format";
                entry = "${pkgs.clang-tools}/bin/clang-format -style=file -i";
                types = [ "text" "c++" ];
                language = "system";
              };
            };
          };
        };

        defaultPackage = self.packages.${system}.kraken;

        devShell = pkgs.mkShell {
          inputsFrom = with self.packages.${system}; [
            kraken
          ];

          inherit (self.checks.${system}.pre-commit) shellHook;
        };

        packages = futils.lib.flattenTree {
          inherit (pkgs) kraken;
        };
      });
}
