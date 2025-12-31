import cutekit as ck
from . import image

import os
from typing import Optional


class Qemu:
    def __init__(self, arch: str, uefi: bool = True):
        self._arch = arch
        self._uefi = uefi

        self.smp = 1
        self.mem = "2G"
        self.bios: Optional[str] = None
        self.display = True
        self.verbose = False

    def hasKvm(self) -> bool:
        return os.path.exists("/dev/kvm") and os.access("/dev/kvm", os.R_OK) and ck.shell.uname().machine == self._arch

    def run(self, img: image.Image):
        qemu_cmd = {
            "x86_64": "qemu-system-x86_64",
        }.get(self._arch)

        if not qemu_cmd:
            raise ValueError(f"Unsupported architecture: {self._arch}")

        cmd = [
            qemu_cmd,
            "-no-reboot",
            "-no-shutdown",
            "-m", self.mem,
        ]

        if self._arch == "x86_64":
            cmd += ["-debugcon", "mon:stdio"]
        else:
            cmd += ["-serial", "mon:stdio"]

        if self.hasKvm() and not self.verbose:
            cmd += ["-enable-kvm"]
        else:
            cmd += ["-accel", "tcg"]

        if self.verbose:
            cmd += ["-d", "int,cpu_reset"]

        if not self.display:
            cmd += ["-display", "none"]

        if self.smp > 1:
            cmd += ["-smp", str(self.smp)]

        if self._uefi:
            assert self.bios is not None, "UEFI BIOS path must be set for UEFI booting."
            cmd += [
                "-drive",
                f"format=raw,file=fat:rw:{img.path},media=disk",
                "-bios", str(self.bios),
            ]
        else:
            raise NotImplementedError("Non-UEFI QEMU booting is not implemented yet.")

        ck.shell.exec(*cmd)
