open TestFramework;

describe("Safe functionality", utils => {
  let checkIfSafe = Reenv.checkSafe(~safeFile="./fixtures/.env");

  utils.test("Should raise if not all keys are there", ({expect}) => {
    expect.fn(() => checkIfSafe(Reenv.make())).toThrow();

    try (checkIfSafe(Reenv.make())) {
    | Reenv.Missing_keys(missingKeys) =>
      expect.string(missingKeys).toMatch("TEST");
      expect.string(missingKeys).toMatch("TEST2");
    // TODO: re-add this test when this is fixed
    // https://github.com/facebookexperimental/reason-native/issues/121
    // expect.string(missingKeys).toMatch("TEST3");
    };
  });

  utils.test("is fine when all the keys are provided", ({expect}) => {
    let env =
      open_in_bin("./fixtures/.env") |> Reenv.t_of_in_channel(Reenv.make());

    expect.same(checkIfSafe(env), ());
  });
});
