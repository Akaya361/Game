Name: Gamelife
Version: 1.0
Release: 1%{?dist}
Summary: Игра "Жизнь"
Group:		Testing
License:	GPL
URL: https://github.com/kostap8/Crosses-zeros
Source: %{name}-%{version}.tar.gz
BuildRoot: /tmp/%{name}

%description
Реализация математической игры "Жизнь" на языке С с использованием графических библиотек GTK и Cairo

%prep
%setup -c %{name}

%build
make

%install
mkdir -p $RPM_BUILD_ROOT/usr/bin
cp %{name} $RPM_BUILD_ROOT/usr/bin

%clean
rm -rf $RPM_BUILD_ROOT

%files
/usr/bin/%{name}

%attr(0755,root,root)/usr/bin/%{name}

%changelog
* Thu May 30 2013 Lubennikova
- Added /usr/bin/Gamelife
