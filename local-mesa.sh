exec env \
    LD_LIBRARY_PATH=$HOME/mesa/lib \
    LIBGL_DRIVERS_PATH=$HOME/mesa/lib/x86_64-linux-gnu/dri \
    GBM_DRIVERS_PATH=$HOME/mesa/lib \
    $@
