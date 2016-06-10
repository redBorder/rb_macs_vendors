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

%prep
%setup -qn %{name}-%{version}

%build
make

%install
mkdir -p %{buildroot}/include
mkdir -p %{buildroot}/lib
DESTDIR=%{buildroot} make install

%clean
rm -rf %{buildroot}

%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig

%files
%defattr(0644,root,root)
/include/rb_mac_vendors.h
/lib/librb_mac_vendors.a
/lib/librb_mac_vendors.so
/lib/librb_mac_vendors.so.0

%changelog
* Fri Jun 10 2016 Carlos J. Mateos <cjmateos@redborder.com> - 1.0.0-1
- first spec version
