require "test_helper"

class PrefixMachineTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::PrefixMachine::VERSION
  end

  def test_basic_machine
    machine = PrefixMachine.new
    machine << 'hello.'
    machine << 'fork.ai.'
    machine << 'fork.human.'
    assert_equal 'hello.', machine.match('hello.world')
    assert_equal 'fork.ai.', machine.match('fork.ai.sdk')
    assert_equal 'fork.human.', machine.match('fork.human.resources')
  end
end
