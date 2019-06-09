class SamplesController < ApplicationController
  before_action :authenticate_user!
  before_action :is_admin
  before_action :set_sample, only: [:edit, :update, :destroy]

  # GET /samples
  # GET /samples.json
  def index
    @samples = Sample.all.order("id")
    @clicks  = Click.all.order("id")
  end

  # GET /samples/new
  def new
    @sample = Sample.find_or_create_by(name: "new sample")
    redirect_to samples_path
  end

  # GET /samples/1/edit
  def edit
  end

  # PATCH/PUT /samples/1
  # PATCH/PUT /samples/1.json
  def update
    respond_to do |format|
      if @sample.update(sample_params)
        format.html { redirect_to samples_path, notice: 'Sample was successfully updated.' }
        format.json { render :show, status: :ok, location: @sample }
      else
        format.html { render :edit }
        format.json { render json: @sample.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /samples/1
  # DELETE /samples/1.json
  def destroy
    @sample.destroy
    respond_to do |format|
      format.html { redirect_to samples_path, notice: 'Sample was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_sample
      @sample = Sample.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def sample_params
      params.require(:sample).permit(:name, :path, :gain, :start, :end, :bpm)
    end

    def is_admin
      if current_user != User.find_by(name: "Mitch")
        redirect_to channels_path
      end
  end
end
