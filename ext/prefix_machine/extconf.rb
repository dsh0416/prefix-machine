require 'mkmf'
extension_name = 'prefix_machine_ext'
dir_config(extension_name)

create_header
create_makefile(extension_name)
