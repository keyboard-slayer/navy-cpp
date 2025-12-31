import cutekit as ck

from . import image

def genConfig(img: image.Image, kernel: str, modules: list[str], *, timeout: int = 0):
    with open(img.path / "limine.conf", "w") as f:
        f.writelines([
            f"timeout: {timeout}\n\n",
            f"/{img.name}\n",
            "    protocol: limine\n",
           f"    kernel_path: boot():{kernel}\n",
        ] + [
            f"    module_path: boot():{mod}\n" for mod in modules
        ])


def downloadLoader(img: image.Image):
    loader = ck.shell.wget("https://codeberg.org/Limine/Limine/raw/branch/v10.x-binary/BOOTX64.EFI")
    img.cp(loader, "EFI/BOOT/BOOTX64.EFI")
