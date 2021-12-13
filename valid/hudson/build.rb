#!/usr/bin/ruby

$LOAD_PATH.push('metabuild/lib')
require 'metabuild'
include Metabuild

arch = "kvx"
options = Options.new({
                        "target"   => {"type" => "keywords", "keywords" => [:kvx], "default" => "kvx", "help" => "Select family architecture."},
                        "march"         => ["k1b:k1bdp,k1bio", "List of target architectures."],
                        "march_valid"   => ["k1b:k1bdp,k1bio", "List of target architectures to validate on execution_platform."],
                        "build_type" => ["Debug", "Build type to use."],
                        "platform"   => ["linux", "Platforms are: 'linux', 'mingw32'."],
                        "kalrayReq"      => ["../kEnv/kvxtools/usr/local/k1Req", "Path to kalrayReq env containing latex stuff."],
                        "version"    => ["unknown", "Version of the delivered ISS core."],
                      })

target = options["target"].to_s

march = options["march"]
march_valid = options["march_valid"]
march_list        = march_list(march)
march_valid_list  = march_list(march_valid)
branch            = options["branch"]

workspace  = options["workspace"]
lao_clone  = options['clone']
lao_path   = File.join(workspace, lao_clone)

repo = Git.new(lao_clone,workspace)

platform   = options["platform"]
build_type = options["build_type"]
host_arch  = `uname -m`.chomp()

kalrayReq      = options["kalrayReq"]
kvxglpk     = kalrayReq
libyaml_prefix = File.join(kalrayReq,"yaml")
softfloat  = File.join(kalrayReq,"softfloat","softfloat-3e")


build_dirs         = []
targets            = []
cmake_dirs_options = []

march_list.each do |target,cores|
  if(platform == "linux") then
    puts "========= Building for Linux. =========\n"
    build_dir = lao_path + "/#{target}_build_#{build_type}"
  elsif(platform == "mingw32") then
    puts "========= Building for Win32. =========\n"
    build_dir = lao_path + "/#{target}_build_Mingwin_#{build_type}"
  else
    raise "Unknown platform: #{platform}"
  end
  targets.push target
  build_dirs.push build_dir
  cmake_dirs_options.push ""
end

clean   = CleanTarget.new("clean", repo, :depends => [])
build   = ParallelTarget.new("build", repo, :depends => [])
install = Target.new("install", repo, :depends => [build])
package = PackageTarget.new("package", repo, :depends => [install])
valid   = Target.new("valid", repo, :depends => [package])


refs    = Target.new("refs", repo, :depends => [valid])

b = Builder.new("lao", options, [clean, build, install, valid, refs, package])

b.logsession = arch

lao_clone       = workspace + "/" + options["clone"]

prefix          = options.fetch("prefix", File.expand_path("none",workspace))
install_prefix  = File.join(prefix,"lao","devimage")
toolroot        = options.fetch("toolroot", File.expand_path("none",workspace))


pkg_prefix_name = options.fetch("pi-prefix-name","#{arch}-")

kalray_internal = File.join(prefix,"kalray_internal")

if (branch != nil && repo.is_same("origin/#{branch}", ".")) then
  b.default_targets = [package]
else
  b.default_targets = [valid]
end

cmake_options =  " -DFAMILY=#{File.join(lao_path,"LAO",target)} "
cmake_options += " -DYAML_ENABLED=ON -DGLPK_ENABLED=ON"
cmake_options += " -DKVX_GLPK=#{kvxglpk}"
cmake_options += " -DLIBYAML_PREFIX=#{libyaml_prefix}"
cmake_options += " -DSOFTFLOAT_PREFIX=#{softfloat}"
cmake_options += " -DKALRAY_INTERNAL=#{kalray_internal}"
cmake_options += " -DCMAKE_INSTALL_PREFIX=#{install_prefix}"

b.target("build") do
  b.logtitle = "Report for LAO build, target = #{target}"

  targets.zip(build_dirs,cmake_dirs_options).each do |target,build_dir, dir_options|

    b.create_goto_dir! build_dir
    b.run("cmake ../LAO #{cmake_options} -DTARGET=#{target} -DCMAKE_BUILD_TYPE=#{build_type} #{dir_options}")
    b.run("make all")
  end
end

b.target("clean") do
  b.logtitle = "Report for LAO clean, target = #{target}"

  build_dirs.each do |build_dir|
    b.run("rm -rf #{build_dir}")
  end
end

b.target("install") do
  b.logtitle = "Report for LAO install, target = #{target}"

  build_dirs.each do |build_dir|
    cd build_dir
    b.run("make install")
  end

  # Copy to toolroot.
  b.rsync(install_prefix,toolroot)
  b.rsync(kalray_internal,File.join(toolroot,"kalray_internal"))
end

b.target("valid") do
  b.logtitle = "Report for LAO valid, target = #{target}"

  targets.zip(build_dirs,cmake_dirs_options).each do |target,build_dir, dir_options|
    b.create_goto_dir! build_dir
    b.run("cmake ../LAO #{cmake_options} -DTARGET=#{target} -DCMAKE_BUILD_TYPE=Debug #{dir_options}")
    b.valid("make check")
  end
end

b.target("refs") do
  b.logtitle = "Report for LAO refs, target = #{target}"

  if(arch == "st200" || arch == "kvx") then
    build_dirs.each do |build_dir|
      cd build_dir
      b.run("make refs")
    end
  end
end

b.target("package") do
  b.logtitle = "Report for LAO packaging, arch = #{host_arch}, type = #{build_type} install = #{install_prefix}"

  # Package: kvx-lao
  cd install_prefix

  package_name = "#{pkg_prefix_name}lao"
  package_file = "#{package_name}.tar"

  b.run("tar cf #{package_file} *")
  tar_package = File.expand_path(package_file)

  package_description = "Kalray LAO (Linear Assembly Optimizer) for #{target} architecture.\n"

  depends = []

  (tools_version,buildID) = options["version"].split("-")
  release_info = b.release_info(tools_version,buildID)
  
  pinfo = b.package_info(package_name, release_info,
                         package_description, depends)
  pinfo.license = "Kalray"
  b.create_package(tar_package, pinfo) unless package.skip
  b.run("rm #{tar_package}")
end

b.launch
