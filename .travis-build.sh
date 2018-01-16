#!/bin/bash
set -ex

image=$1

if [[ $image =~ ^centos: ]]
then
    pkgcmd="yum"
    builddep="yum-builddep"
    sed -i '/^tsflags=/d' /etc/yum.conf
    yum install -y epel-release
    yum install -y @buildsys-build
    yum install -y yum-utils
    yum install -y yum-plugin-copr
    yum install -y git
    yum copr enable -y pat1/simc
elif [[ $image =~ ^fedora: ]]
then
    pkgcmd="dnf"
    builddep="dnf builddep"
    sed -i '/^tsflags=/d' /etc/dnf/dnf.conf
    dnf install -y @buildsys-build
    dnf install -y 'dnf-command(builddep)'
    dnf install -y git
    dnf copr enable -y pat1/simc
fi

$builddep -y fedora/SPECS/meteo-vm2.spec

if [[ $image =~ ^fedora: || $image =~ ^centos: ]]
then
    pkgname="$(rpmspec -q --qf="meteo-vm2-%{version}-%{release}\n" fedora/SPECS/meteo-vm2.spec | head -n1)"
    mkdir -p ~/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
    cp fedora/SPECS/meteo-vm2.spec ~/rpmbuild/SPECS/meteo-vm2.spec
    git archive --prefix=$pkgname/ --format=tar HEAD | gzip -c > ~/rpmbuild/SOURCES/$pkgname.tar.gz
    rpmbuild -ba ~/rpmbuild/SPECS/meteo-vm2.spec
    find ~/rpmbuild/{RPMS,SRPMS}/ -name "${pkgname}*rpm" -exec cp -v {} . \;
    # TODO upload ${pkgname}*.rpm to github release on deploy stage
else
    autoreconf -ifv
    ./configure
    make check
fi