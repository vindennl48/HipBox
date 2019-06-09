require 'test_helper'

class WifisControllerTest < ActionDispatch::IntegrationTest
  setup do
    @wifi = wifis(:one)
  end

  test "should get index" do
    get wifis_url
    assert_response :success
  end

  test "should get new" do
    get new_wifi_url
    assert_response :success
  end

  test "should create wifi" do
    assert_difference('Wifi.count') do
      post wifis_url, params: { wifi: { name: @wifi.name, network: @wifi.network, password: @wifi.password } }
    end

    assert_redirected_to wifi_url(Wifi.last)
  end

  test "should show wifi" do
    get wifi_url(@wifi)
    assert_response :success
  end

  test "should get edit" do
    get edit_wifi_url(@wifi)
    assert_response :success
  end

  test "should update wifi" do
    patch wifi_url(@wifi), params: { wifi: { name: @wifi.name, network: @wifi.network, password: @wifi.password } }
    assert_redirected_to wifi_url(@wifi)
  end

  test "should destroy wifi" do
    assert_difference('Wifi.count', -1) do
      delete wifi_url(@wifi)
    end

    assert_redirected_to wifis_url
  end
end
