{
  description = "INET Framework for OMNeT++ Discrete Event Simulator";

  inputs = {
    omnetpp.url = "github:omnetpp/omnetpp/flake/omnetpp-6.0.x";
    # use the same version of nixpkgs and nix-utils as the omnetpp flake to minimize dependency size
    nixpkgs.follows = "omnetpp/nixpkgs";
    flake-utils.follows = "omnetpp/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, omnetpp }:
    flake-utils.lib.eachDefaultSystem(system:
    let
      pname = "inet";
      version = "4.4.0.${nixpkgs.lib.substring 0 8 self.lastModifiedDate}.${self.shortRev or "dirty"}";
      pkgs = import nixpkgs { inherit system; };
      oppCallPackage = omnetpp.oppCallPackage.${system};
    in rec {
      packages = rec {
        ${pname} = oppCallPackage ./releng/inet_mkDerivation.nix {
          inherit pname version;
          omnetpp = omnetpp.packages.${system}.omnetpp;
          src = self;
        };

        default = packages.${pname};
      };

      devShells = rec {
        "${pname}-dev" = pkgs.mkShell {
          name = "inet-dev";
          buildInputs = [
            pkgs.hello
            omnetpp.packages.${system}.default
            self.packages.${system}.default
          ];
        };

        default = devShells."${pname}-dev";
      };
    });
}