import cutekit as ck

git_url = "https://codeberg.org/OSDev/freestnd-cxx-hdrs"
branch = "trunk"
kind = "lib"

def package(scope: ck.model.PortScope):
    src = scope.srcDir / scope.target.props['arch'] / "include"

    if not src.exists():
        raise RuntimeError(f"Source directory {src} does not exist.")

    ck.shell.cpTree(str(src), str(scope.includeDir))
