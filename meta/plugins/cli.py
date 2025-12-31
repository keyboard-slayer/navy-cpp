import cutekit as ck

from . import image
from . import limine
from . import qemu


@ck.cli.command("image", "Generate a boot image")
def _(): ...


@ck.cli.command("image/start", "Run a boot image inside of QEMU")
def _(args: ck.model.RegistryArgs):
    registry = ck.model.Registry.use(args)
    img = image.Image(registry, "navy")
    img.install("kernel.core", "kernel-x86_64", "/kernel.elf")

    limine.genConfig(img, "/kernel.elf", [])
    limine.downloadLoader(img)

    vm = qemu.Qemu("x86_64")
    vm.bios = ck.shell.wget("https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF.fd")
    vm.display = False
    vm.run(img)
