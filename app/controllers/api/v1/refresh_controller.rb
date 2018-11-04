class Api::V1::RefreshController < Api::V1::BaseController
  def index
    result = {
      james: {
        solo: false
      },
      Jesse: {
        solo: false
      },
      Sean: {
        solo: false
      },
    }
    render json: result
  end
end
