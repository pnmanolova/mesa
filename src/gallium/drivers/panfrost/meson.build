# Copyright © 2017 Intel Corporation
# Copyright © 2018 Alyssa Rosenzweig

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

files_panfrost = files(
  'pan_public.h',
  'pan_screen.c',
  'pan_screen.h',
  'pan_resource.c',
  'pan_resource.h',

  'midgard/midgard_compile.c',
  'midgard/cppwrap.cpp',
  'midgard/disassemble.c',

  'pan_context.c',
  'pan_job.c',
  'pan_trace.c',
  'pan_drm.c',
  'pan_allocate.c',
  'pan_assemble.c',
  'pan_format.c',
  'pan_swizzle.c',
  'pan_blending.c',
  'pan_blend_shaders.c',
  'pan_wallpaper.c',
  'pan_pretty_print.c',
)

inc_panfrost = [
  inc_common,
  inc_gallium,
  inc_gallium_aux,
  inc_include,
  inc_src,
  include_directories('include'),
  include_directories('midgard'),
]

compile_args_panfrost = [
  '-DGALLIUM_PANFROST',
  '-Wno-pointer-arith'
]

overlay = join_paths(meson.source_root(), meson.current_source_dir(), 'nondrm/pan_nondrm.c')
nondrm_overlay_check = run_command('ls', overlay)
has_nondrm_overlay = nondrm_overlay_check.returncode() == 0

if has_nondrm_overlay
  files_panfrost += files('nondrm/pan_nondrm.c')
  inc_panfrost += include_directories('nondrm/include')
  compile_args_panfrost += '-DPAN_NONDRM_OVERLAY'
endif

midgard_nir_algebraic_c = custom_target(
  'midgard_nir_algebraic.c',
  input : 'midgard/midgard_nir_algebraic.py',
  output : 'midgard_nir_algebraic.c',
  command : [
    prog_python, '@INPUT@',
    '-p', join_paths(meson.source_root(), 'src/compiler/nir/'),
  ],
  capture : true,
  depend_files : nir_algebraic_py,
)

libpanfrost = static_library(
  'panfrost',
  [files_panfrost, midgard_nir_algebraic_c],
  dependencies: [
    dep_thread,
    dep_libdrm,
    idep_nir
  ],
  include_directories : inc_panfrost,
  c_args : [c_vis_args, c_msvc_compat_args, compile_args_panfrost],
)

driver_panfrost = declare_dependency(
  compile_args : compile_args_panfrost,
  link_with : [libpanfrost, libpanfrostwinsys],
)

files_midgard = files(
  'midgard/midgard_compile.c',
  'midgard/cppwrap.cpp',
  'midgard/disassemble.c',
  'midgard/cmdline.c',
)

midgard_compiler = executable(
  'midgard_compiler',
  [files_midgard, midgard_nir_algebraic_c],
  include_directories : inc_panfrost,
  dependencies : [
    dep_thread,
    idep_nir
  ],
  link_with : [
    libgallium,
    libglsl_standalone,
    libmesa_util
  ],
  build_by_default : true
)

files_pandecode = files(
  'pandecode/cmdline.c',
  'pandecode/decode.c',

  'pan_pretty_print.c',

  'midgard/disassemble.c'
)

pandecode = executable(
  'pandecode',
  files_pandecode,
  include_directories : inc_panfrost,
  dependencies : [
    dep_thread,
  ],
  link_with : [
    libmesa_util
  ],
  build_by_default : true
)
