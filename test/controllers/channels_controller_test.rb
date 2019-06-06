require 'test_helper'

class ChannelsControllerTest < ActionDispatch::IntegrationTest
  test "should get index" do
    get channels_index_url
    assert_response :success
  end

end
