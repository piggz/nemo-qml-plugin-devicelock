Name:       nemo-qml-plugin-devicelock
Summary:    Device lock plugin for Nemo Mobile
Version:    0.0.0
Release:    1
Group:      System/Libraries
License:    LGPLv2.1
URL:        https://git.merproject.org/mer-core/nemo-qml-plugin-dbus
Source0:    %{name}-%{version}.tar.bz2
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(keepalive)
BuildRequires:  pkgconfig(mce)
BuildRequires:  pkgconfig(mlite5)
BuildRequires:  pkgconfig(libsystemd-daemon)
Obsoletes:      nemo-qml-plugin-devicelock-default
Requires:       nemo-devicelock-daemon

%description
%{summary}.

%package -n nemo-devicelock-daemon-cli
Summary:    The default command line lock code device lock daemon for Nemo Mobile
Group:      System/GUI/Other
Requires:   %{name} = %{version}
Provides:   nemo-devicelock-daemon

%description -n nemo-devicelock-daemon-cli
%{summary}.

%package devel
Summary:    Development libraries for device lock
Group:      Development/Libraries
Requires:   %{name} = %{version}

%description devel
%{summary}.

%package host-devel
Summary:    Development libraries for device lock daemons
Group:      Development/Libraries
Requires:   %{name}-devel = %{version}

%description host-devel
%{summary}.

%prep
%setup -q -n %{name}-%{version}

%build
%qmake5
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%qmake5_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libnemodevicelock.so.*
%dir %{_libdir}/qt5/qml/org/nemomobile/devicelock
%{_libdir}/qt5/qml/org/nemomobile/devicelock/libnemodevicelockplugin.so
%{_libdir}/qt5/qml/org/nemomobile/devicelock/qmldir

%files -n nemo-devicelock-daemon-cli
%defattr(-,root,root,-)
%{_bindir}/nemodevicelockd
/lib/systemd/system/nemodevicelock.service
/lib/systemd/system/nemodevicelock.socket
%config %{_sysconfdir}/dbus-1/system.d/org.nemomobile.devicelock.conf

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/nemo-devicelock
%{_includedir}/nemo-devicelock/*.h
%{_includedir}/nemo-devicelock/private/*.h
%{_includedir}/nemo-devicelock/host/*.h
%{_libdir}/libnemodevicelock.so
%{_libdir}/pkgconfig/nemodevicelock.pc

%files host-devel
%defattr(-,root,root,-)
%dir %{_includedir}/nemo-devicelock/host
%{_includedir}/nemo-devicelock/host/*.h
%{_libdir}/libnemodevicelock-host.a
