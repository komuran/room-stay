require 'test_helper'

class CapturesControllerTest < ActionDispatch::IntegrationTest
  test "should get new" do
    get captures_new_url
    assert_response :success
  end

end
