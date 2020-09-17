require "bundler/gem_tasks"
require "rake/testtask"
require 'rake/extensiontask'

spec = Gem::Specification.load('prefix-machine.gemspec')
Rake::ExtensionTask.new('prefix_machine_ext', spec) do |ext|
  ext.ext_dir = "ext/prefix_machine"
end

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

task :default => :test
