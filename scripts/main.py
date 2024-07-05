def _check_pkg(target):
    """
    Return name, version and if rpm package for specified target
    """
    ret = {}
    cmd = ["/usr/bin/lslpp", "-Lc", target]
    result = __salt__["cmd.run_all"](cmd, python_shell=False)

    if 0 == result["retcode"]:
        name = ""
        version_num = ""
        rpmpkg = False
        lines = result["stdout"].splitlines()
        for line in lines:
            if line.startswith("#"):
                continue

            comps = line.split(":")
            if len(comps) < 7:
                raise CommandExecutionError(
                    "Error occurred finding fileset/package",
                    info={"errors": comps[1].strip()},
                )

            # handle first matching line
            if "R" in comps[6]:
                name = comps[0]
                rpmpkg = True
            else:
                name = comps[1]  # use fileset rather than rpm package

            version_num = comps[2]
            break

        return name, version_num, rpmpkg
    else:
        raise CommandExecutionError(
            "Error occurred finding fileset/package",
            info={"errors": result["stderr"].strip()},
        )


def _is_installed_rpm(name):
    """
    Returns True if the rpm package is installed. Otherwise returns False.
    """
    cmd = ["/usr/bin/rpm", "-q", name]
    return __salt__["cmd.retcode"](cmd) == 0


def _list_pkgs_from_context(versions_as_list):
    """
    Use pkg list from __context__
    """
    if versions_as_list:
        return __context__["pkg.list_pkgs"]
    else:
        ret = copy.deepcopy(__context__["pkg.list_pkgs"])
        __salt__["pkg_resource.stringify"](ret)
        return ret

