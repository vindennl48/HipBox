require 'test_helper'

class PortGroupsControllerTest < ActionDispatch::IntegrationTest
  test "should get edit" do
    get port_groups_edit_url
    assert_response :success
  end

  test "should get update" do
    get port_groups_update_url
    assert_response :success
  end

end
