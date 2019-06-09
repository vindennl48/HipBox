require "application_system_test_case"

class WifisTest < ApplicationSystemTestCase
  setup do
    @wifi = wifis(:one)
  end

  test "visiting the index" do
    visit wifis_url
    assert_selector "h1", text: "Wifis"
  end

  test "creating a Wifi" do
    visit wifis_url
    click_on "New Wifi"

    fill_in "Name", with: @wifi.name
    fill_in "Network", with: @wifi.network
    fill_in "Password", with: @wifi.password
    click_on "Create Wifi"

    assert_text "Wifi was successfully created"
    click_on "Back"
  end

  test "updating a Wifi" do
    visit wifis_url
    click_on "Edit", match: :first

    fill_in "Name", with: @wifi.name
    fill_in "Network", with: @wifi.network
    fill_in "Password", with: @wifi.password
    click_on "Update Wifi"

    assert_text "Wifi was successfully updated"
    click_on "Back"
  end

  test "destroying a Wifi" do
    visit wifis_url
    page.accept_confirm do
      click_on "Destroy", match: :first
    end

    assert_text "Wifi was successfully destroyed"
  end
end
