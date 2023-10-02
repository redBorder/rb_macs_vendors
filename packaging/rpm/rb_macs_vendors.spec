Name:	  rb_macs_vendors
Version:  %{__version}
Release:  %{__release}%{?dist}

License:  GNU AGPLv3	
URL:  https://github.com/redBorder/rb_macs_vendors
Source0: %{name}-%{version}.tar.gz

BuildRequires: gcc librd-devel

Summary: Redborder library that get a mac and identify mac's vendor
Group: Development/Libraries/C and C++
Requires: librd0

%description
%{summary}

%global debug_package %{nil}

%prep
%setup -qn %{name}-%{version}

%build
make

%install
mkdir -p %{buildroot}/usr/include
mkdir -p %{buildroot}/usr/lib
DESTDIR=%{buildroot}/usr make install

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(0644,root,root)
/usr/include/rb_mac_vendors.h
/usr/lib/librb_mac_vendors.a
/usr/lib/librb_mac_vendors.so
/usr/lib/librb_mac_vendors.so.0

%changelog
* Fri Jun 10 2016 Carlos J. Mateos <cjmateos@redborder.com> - 1.0.0-1
- first spec version
