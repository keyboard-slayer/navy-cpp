import cutekit as ck

from pathlib import Path

class Image:
    def __init__(self, registry: ck.model.Registry, name: str):
        self._registry = registry
        self._name = name

    @property
    def path(self) -> Path:
        return Path(ck.const.PROJECT_CK_DIR) / "images" / self._name

    @property
    def name(self) -> str:
        return self._name

    def build(self, component: str, target: str) -> Path:
        comp = self._registry.lookup(component, ck.model.Component)
        assert comp is not None, f"Component '{component}' not found in registry."

        targ = self._registry.lookup(target, ck.model.Target)
        assert targ is not None, f"Target '{target}' not found in registry."

        scope = ck.builder.TargetScope(self._registry, targ)
        return Path(ck.builder.build(scope, comp)[0].path)

    def install(self, component: str, target: str, destination: str) -> None:
        binPath = self.build(component, target)
        imgPath = self.path / destination.removeprefix("/")
        imgPath.parent.mkdir(parents=True, exist_ok=True)
        ck.shell.cp(str(binPath), str(imgPath))

    def cp(self, source: str, destination: str) -> None:
        srcPath = Path(source)
        destPath = self.path / destination.removeprefix("/")
        destPath.parent.mkdir(parents=True, exist_ok=True)
        ck.shell.cp(str(srcPath), str(destPath))
