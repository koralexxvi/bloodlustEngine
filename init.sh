set -e
if [ -d "./build" ]; then
	meson setup --reconfigure build
else
	meson setup build
fi
