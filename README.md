# prefix-machine
A High Performance Prefix Match Machine for Ruby

```ruby
require 'prefix-machine'

machine = PrefixMachine.new
machine << 'hello.'
machine << 'fork.ai.'
machine << 'fork.human.'
machine.match('hello.world') # => 'hello.'
machine.match('fork.ai.sdk') # => 'fork.ai.'
machine.match('fork.human.resources') # => 'fork.human.'
```
