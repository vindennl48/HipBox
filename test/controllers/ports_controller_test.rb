require 'test_helper'

class PortsControllerTest < ActionDispatch::IntegrationTest
  test "should get index" do
    get ports_index_url
    assert_response :success
  end

  test "should get edit" do
    get ports_edit_url
    assert_response :success
  end

  test "should get update" do
    get ports_update_url
    assert_response :success
  end

end
