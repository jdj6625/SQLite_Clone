require "rspec"
require "open3"

# Returns full path to built SQLite executable
def db_exe
  root = File.expand_path("../..", __dir__)
  exe = File.join(root, "cmake-build-debug", "SQLite.exe")

  unless File.exist?(exe)
    raise "Database executable not found at: #{exe}"
  end

  exe
end

# Runs SQLite with given commands and returns stdout split into lines
def run_script(commands)
  input = commands.join("\n") + "\n"

  stdout, status = Open3.capture2(db_exe, stdin_data: input)

  unless status.success?
    raise "Database exited with status #{status.exitstatus}"
  end

  stdout.split("\n")
end

RSpec.describe "database" do
  it "inserts and retrieves a row" do
    result = run_script([
      "insert 1 user1 person1@example.com",
      "select",
      ".exit",
    ])

    # Adjust this line if your printRow format changes
    expect(result).to include("db > Executed.")
    expect(result).to include(
      "db > Id: 1, Username: user1, Email: person1@example.com"
    )
  end

  it 'prints error message when table is full' do
    script = (1..1401).map do |i|
      "insert #{i} user#{i} person#{i}@example.com"
    end
    script << ".exit"
    result = run_script(script)
    expect(result[-2]).to eq('db > Error: Table full.')
  end

  it 'allows inserting strings that are the maximum length' do
    long_username = "a"*32
    long_email = "a"*255
    script = [
      "insert 1 #{long_username} #{long_email}",
      "select",
      ".exit",
    ]
    result = run_script(script)
    expect(result).to match_array([
      "db > Executed.",
      "db > (1, #{long_username}, #{long_email})",
      "Executed.",
      "db > ",
    ])
  end

end

